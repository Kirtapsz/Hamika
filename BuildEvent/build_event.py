import logging
import shutil
import os
import sys
import subprocess

logging.basicConfig(format='%(asctime)s - %(message)s', level=logging.INFO)

logging.info(f'Args: {sys.argv}')

event_type = sys.argv[1]
POST_BUILD = event_type == "post"
PRE_BUILD = event_type == "pre"

if (POST_BUILD):
    logging.info('PostBuild script running...')
if (PRE_BUILD):
    logging.info('PreBuild script running...')

project_dir = sys.argv[2]

kirlib = os.environ['KIRLIB']
RELEASE_BUILD = sys.argv[3] == "Release"

if (POST_BUILD):
    out_dir = sys.argv[4]

    os.chdir(out_dir)
    
    src = os.path.join(project_dir, "Hamika")
    dst = os.path.join(out_dir, "Hamika")
    shutil.rmtree(dst, ignore_errors=True)
    os.makedirs(dst, exist_ok=True)
    shutil.copytree(src, dst, dirs_exist_ok=True)

ret = subprocess.run(["git" ,"-C" ,kirlib ,"rev-parse" ,"HEAD"], capture_output=True)
if (ret.returncode != 0):
    raise Exception("Git command failed")
stdout = ret.stdout.decode("utf-8")
kirlib_commit_id = stdout.split("\n")[0]
logging.info(f'KIRLIB commit: {kirlib_commit_id}')

ret = subprocess.run(["git" ,"-C" ,kirlib ,"status"], capture_output=True)
if (ret.returncode != 0):
    raise Exception("Git command failed")
stdout = ret.stdout.decode("utf-8")
kirlib_commited = "nothing to commit, working tree clean" in stdout
logging.info(f'KIRLIB commit status: {kirlib_commited}')

ret = subprocess.run(["git" ,"-C" ,project_dir ,"rev-parse" ,"HEAD"], capture_output=True)
if (ret.returncode != 0):
    raise Exception("Git command failed")
stdout = ret.stdout.decode("utf-8")
hamika_commit_id = stdout.split("\n")[0]
logging.info(f'Hamika commit: {hamika_commit_id}')

ret = subprocess.run(["git" ,"-C" ,project_dir ,"status"], capture_output=True)
if (ret.returncode != 0):
    raise Exception("Git command failed")
stdout = ret.stdout.decode("utf-8")
hamika_commited = "nothing to commit, working tree clean" in stdout
logging.info(f'Hamika commit status: {hamika_commited}')

if (POST_BUILD and RELEASE_BUILD):
    if (hamika_commited == False):
        raise Exception("Hamika Changes are not commited")
    if (kirlib_commited == False):
        raise Exception("KIRLIB changes are not commited")
    validators = "Hamika\\multitest\\validators"
    ext = ('.json')
    for file in os.listdir(validators):
        if file.endswith(ext):
            subprocess.run(["Hamika.exe", "--multitest", "-json", os.path.join(validators,file)], shell=True, check=True)

if (PRE_BUILD):
    f = open(os.path.join(project_dir, "versions.h"), "w")
    f.write(f"""#pragma once

namespace Versions
{'{'}
    constexpr char *ApplicationCommitID = "{hamika_commit_id}";
    constexpr char *KIRLIBCommitID = "{kirlib_commit_id}";
{'}'}
""")
    f.close()

if (POST_BUILD):
    logging.info('PostBuild script done...')
if (PRE_BUILD):
    logging.info('PreBuild script done...')