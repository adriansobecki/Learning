import subprocess
import os
import argparse
import shutil

executableName = "modelBased.exe"
generator = "\"MinGW Makefiles\""

sourceDir = os.path.abspath(os.path.dirname(__file__))
buildDir = os.path.join(sourceDir, "build")
binDir = os.path.join(sourceDir, "bin")

def clean():
    #cleanCommand = ["cmake", "--build", "build", "--target", "clean"]
    #subprocess.run(cleanCommand, check = True)

    if os.path.exists(buildDir): shutil.rmtree(buildDir)
    if os.path.exists(binDir): shutil.rmtree(binDir)

def build(amountOfThreads):
    # Create a directory if it doesn't exist
    os.makedirs(buildDir, exist_ok = True)
    os.makedirs(binDir, exist_ok = True)

    executableFileLocation = os.path.join(binDir, executableName)
    if os.path.exists(executableFileLocation): os.remove(executableFileLocation)

    # Change the working directory to the build directory
    os.chdir(buildDir)

    cmakeCommand = f"cmake -G {generator} ../"
    subprocess.run(cmakeCommand, check = True)

    makeCommand = f"make -j {amountOfThreads}"
    subprocess.run(makeCommand, check = True)

    print("Compilation completed.")

def install():
    installCommand = f"cmake --install ."
    subprocess.run(installCommand, check = True)

    print("Instalation completed.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Your script description')

    # Add required flags (-m or -b)
    group = parser.add_mutually_exclusive_group(required = True)
    group.add_argument('-m', '--make', action = 'store_true')
    group.add_argument('-b', '--build', action = 'store_true')

    parser.add_argument('-j', '--jobs', default = 4)
    # Parse the command line arguments
    args = parser.parse_args()

    if args.build == True: clean()

    build(args.jobs)

    install()