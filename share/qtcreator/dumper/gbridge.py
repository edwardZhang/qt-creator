
import binascii
import inspect
import os
import traceback
import gdb

cdbLoaded = False
lldbLoaded = False
gdbLoaded = False

#######################################################################
#
# Infrastructure
#
#######################################################################

def warn(message):
    print "XXX: %s\n" % message.encode("latin1")

def savePrint(output):
    try:
        print(output)
    except:
        out = ""
        for c in output:
            cc = ord(c)
            if cc > 127:
                out += "\\\\%d" % cc
            elif cc < 0:
                out += "\\\\%d" % (cc + 256)
            else:
                out += c
        print(out)

def registerCommand(name, func):

    class Command(gdb.Command):
        def __init__(self):
            super(Command, self).__init__(name, gdb.COMMAND_OBSCURE)
        def invoke(self, args, from_tty):
            savePrint(func(args))

    Command()

def parseAndEvaluate(exp):
    return gdb.parse_and_eval(exp)

def extractFields(value):
    return value.type.fields()
    ## Insufficient, see http://sourceware.org/bugzilla/show_bug.cgi?id=10953:
    ##fields = type.fields()
    ## Insufficient, see http://sourceware.org/bugzilla/show_bug.cgi?id=11777:
    ##fields = defsype).fields()
    ## This seems to work.
    ##warn("TYPE 0: %s" % type)
    #type = stripTypedefs(type)
    #fields = type.fields()
    #if len(fields):
    #    return fields
    ##warn("TYPE 1: %s" % type)
    ## This fails for arrays. See comment in lookupType.
    #type0 = lookupType(str(type))
    #if not type0 is None:
    #    type = type0
    #if type.code == FunctionCode:
    #    return []
    ##warn("TYPE 2: %s" % type)
    #fields = type.fields()
    ##warn("FIELDS: %s" % fields)
    #return fields

def fieldCount(type):
    return len(type.fields())

def listOfLocals(varList):
    frame = gdb.selected_frame()
    try:
        frame = gdb.selected_frame()
        #warn("FRAME %s: " % frame)
    except RuntimeError, error:
        warn("FRAME NOT ACCESSIBLE: %s" % error)
        return []
    except:
        warn("FRAME NOT ACCESSIBLE FOR UNKNOWN REASONS")
        return []

    try:
        block = frame.block()
        #warn("BLOCK: %s " % block)
    except RuntimeError, error:
        warn("BLOCK IN FRAME NOT ACCESSIBLE: %s" % error)
        return []
    except:
        warn("BLOCK NOT ACCESSIBLE FOR UNKNOWN REASONS")
        return []

    items = []
    shadowed = {}
    while True:
        if block is None:
            warn("UNEXPECTED 'None' BLOCK")
            break
        for symbol in block:
            name = symbol.print_name

            if name == "__in_chrg":
                continue

            # "NotImplementedError: Symbol type not yet supported in
            # Python scripts."
            #warn("SYMBOL %s  (%s): " % (symbol, name))
            if name in shadowed:
                level = shadowed[name]
                name1 = "%s@%s" % (name, level)
                shadowed[name] = level + 1
            else:
                name1 = name
                shadowed[name] = 1
            #warn("SYMBOL %s  (%s, %s)): " % (symbol, name, symbol.name))
            item = LocalItem()
            item.iname = "local." + name1
            item.name = name1
            try:
                item.value = frame.read_var(name, block)
                #warn("READ 1: %s" % item.value)
                if not item.value.is_optimized_out:
                    #warn("ITEM 1: %s" % item.value)
                    items.append(item)
                    continue
            except:
                pass

            try:
                item.value = frame.read_var(name)
                #warn("READ 2: %s" % item.value)
                if not item.value.is_optimized_out:
                    #warn("ITEM 2: %s" % item.value)
                    items.append(item)
                    continue
            except:
                # RuntimeError: happens for
                #     void foo() { std::string s; std::wstring w; }
                # ValueError: happens for (as of 2010/11/4)
                #     a local struct as found e.g. in
                #     gcc sources in gcc.c, int execute()
                pass

            try:
                #warn("READ 3: %s %s" % (name, item.value))
                item.value = gdb.parse_and_eval(name)
                #warn("ITEM 3: %s" % item.value)
                items.append(item)
            except:
                # Can happen in inlined code (see last line of
                # RowPainter::paintChars(): "RuntimeError:
                # No symbol \"__val\" in current context.\n"
                pass

        # The outermost block in a function has the function member
        # FIXME: check whether this is guaranteed.
        if not block.function is None:
            break

        block = block.superblock

    return items


def catchCliOutput(command):
    try:
        return gdb.execute(command, to_string=True).split("\n")
    except:
        pass
    filename = createTempFile()
    gdb.execute("set logging off")
#        gdb.execute("set logging redirect off")
    gdb.execute("set logging file %s" % filename)
#        gdb.execute("set logging redirect on")
    gdb.execute("set logging on")
    msg = ""
    try:
        gdb.execute(command)
    except RuntimeError, error:
        # For the first phase of core file loading this yield
        # "No symbol table is loaded.  Use the \"file\" command."
        msg = str(error)
    except:
        msg = "Unknown error"
    gdb.execute("set logging off")
#        gdb.execute("set logging redirect off")
    if len(msg):
        # Having that might confuse result handlers in the gdbengine.
        #warn("CLI ERROR: %s " % msg)
        removeTempFile(filename)
        return "CLI ERROR: %s " % msg
    temp = open(filename, "r")
    lines = []
    for line in temp:
        lines.append(line)
    temp.close()
    removeTempFile(filename)
    return lines

def selectedInferior():
    try:
        # Does not exist in 7.3.
        return gdb.selected_inferior()
    except:
        pass
    # gdb.Inferior is new in gdb 7.2
    return gdb.inferiors()[0]

def readRawMemory(base, size):
    try:
        inferior = selectedInferior()
        return binascii.hexlify(inferior.read_memory(base, size))
    except:
        pass
    s = ""
    t = lookupType("unsigned char").pointer()
    base = base.cast(t)
    for i in xrange(size):
        s += "%02x" % int(base.dereference())
        base += 1
    return s


#######################################################################
#
# Types
#
#######################################################################

PointerCode = gdb.TYPE_CODE_PTR
ArrayCode = gdb.TYPE_CODE_ARRAY
StructCode = gdb.TYPE_CODE_STRUCT
UnionCode = gdb.TYPE_CODE_UNION
EnumCode = gdb.TYPE_CODE_ENUM
FlagsCode = gdb.TYPE_CODE_FLAGS
FunctionCode = gdb.TYPE_CODE_FUNC
IntCode = gdb.TYPE_CODE_INT
FloatCode = gdb.TYPE_CODE_FLT # Parts of GDB assume that this means complex.
VoidCode = gdb.TYPE_CODE_VOID
#SetCode = gdb.TYPE_CODE_SET
RangeCode = gdb.TYPE_CODE_RANGE
StringCode = gdb.TYPE_CODE_STRING
#BitStringCode = gdb.TYPE_CODE_BITSTRING
#ErrorTypeCode = gdb.TYPE_CODE_ERROR
MethodCode = gdb.TYPE_CODE_METHOD
MethodPointerCode = gdb.TYPE_CODE_METHODPTR
MemberPointerCode = gdb.TYPE_CODE_MEMBERPTR
ReferenceCode = gdb.TYPE_CODE_REF
CharCode = gdb.TYPE_CODE_CHAR
BoolCode = gdb.TYPE_CODE_BOOL
ComplexCode = gdb.TYPE_CODE_COMPLEX
TypedefCode = gdb.TYPE_CODE_TYPEDEF
NamespaceCode = gdb.TYPE_CODE_NAMESPACE
#Code = gdb.TYPE_CODE_DECFLOAT # Decimal floating point.
#Code = gdb.TYPE_CODE_MODULE # Fortran
#Code = gdb.TYPE_CODE_INTERNAL_FUNCTION
SimpleValueCode = -1


#######################################################################
#
# Step Command
#
#######################################################################

def sal(args):
    (cmd, addr) = args.split(",")
    lines = catchCliOutput("info line *" + addr)
    fromAddr = "0x0"
    toAddr = "0x0"
    for line in lines:
        pos0from = line.find(" starts at address") + 19
        pos1from = line.find(" ", pos0from)
        pos0to = line.find(" ends at", pos1from) + 9
        pos1to = line.find(" ", pos0to)
        if pos1to > 0:
            fromAddr = line[pos0from : pos1from]
            toAddr = line[pos0to : pos1to]
    gdb.execute("maint packet sal%s,%s,%s" % (cmd,fromAddr, toAddr))

registerCommand("sal", sal)


#######################################################################
#
# Convenience
#
#######################################################################

# Just convienience for 'python print ...'
class PPCommand(gdb.Command):
    def __init__(self):
        super(PPCommand, self).__init__("pp", gdb.COMMAND_OBSCURE)
    def invoke(self, args, from_tty):
        print(eval(args))

PPCommand()

# Just convienience for 'python print gdb.parse_and_eval(...)'
class PPPCommand(gdb.Command):
    def __init__(self):
        super(PPPCommand, self).__init__("ppp", gdb.COMMAND_OBSCURE)
    def invoke(self, args, from_tty):
        print(gdb.parse_and_eval(args))

PPPCommand()


def scanStack(p, n):
    p = long(p)
    r = []
    for i in xrange(n):
        f = gdb.parse_and_eval("{void*}%s" % p)
        m = gdb.execute("info symbol %s" % f, to_string=True)
        if not m.startswith("No symbol matches"):
            r.append(m)
        p += f.type.sizeof
    return r

class ScanStackCommand(gdb.Command):
    def __init__(self):
        super(ScanStackCommand, self).__init__("scanStack", gdb.COMMAND_OBSCURE)
    def invoke(self, args, from_tty):
        if len(args) == 0:
            args = 20
        savePrint(scanStack(gdb.parse_and_eval("$sp"), int(args)))

ScanStackCommand()


gdbLoaded = True
currentDir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
execfile(os.path.join(currentDir, "dumper.py"))
execfile(os.path.join(currentDir, "qttypes.py"))
bbsetup()