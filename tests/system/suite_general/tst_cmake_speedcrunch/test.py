source("../../shared/qtcreator.py")

SpeedCrunchPath = ""
BuildPath = tempDir()

def main():
    if (which("cmake") == None):
        test.fatal("cmake not found in PATH - needed to run this test")
        return
    if (which("qmake") == None):
        test.fatal("qmake not found in PATH - needed to run this test")
        return
    if not neededFilePresent(SpeedCrunchPath):
        return

    startApplication("qtcreator" + SettingsPath)
    if not startedWithoutPluginError():
        return
    result = openCmakeProject(SpeedCrunchPath, BuildPath)
    if not result:
        test.fatal("Could not open/create cmake project - leaving test")
        invokeMenuItem("File", "Exit")
        return
    waitForSignal("{type='CppTools::Internal::CppModelManager' unnamed='1'}", "sourceFilesRefreshed(QStringList)")

    # Invoke a rebuild of the application
    invokeMenuItem("Build", "Rebuild All")

    # Wait for, and test if the build succeeded
    waitForSignal("{type='ProjectExplorer::BuildManager' unnamed='1'}", "buildQueueFinished(bool)", 300000)
    checkCompile()
    checkLastBuild()

    invokeMenuItem("File", "Exit")

def init():
    global SpeedCrunchPath
    SpeedCrunchPath = srcPath + "/creator-test-data/speedcrunch/src/CMakeLists.txt"
    cleanup()

def cleanup():
    global BuildPath
    # Make sure the .user files are gone
    cleanUpUserFiles(SpeedCrunchPath)
    deleteDirIfExists(BuildPath)
