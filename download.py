import os
import json
import hashlib

download_url="https://autopatchcn.yuanshen.com/client_app/download/pc_zip/20230625120029_C0NLGkC0fxSaNKnu/ScatteredFiles/"

def verify_file(filename,hash):
    try:
        pend_file=open(filename,'rb')
    except:
        return False
    chk_file=pend_file.read()
    hash_res=hashlib.md5(chk_file).hexdigest()
    pend_file.close()
    if(hash_res==hash):
        return True
    else:
        return False


os.system("curl -O "+download_url+"pkg_version")
pkg=open("pkg_version")
# pkg=open("Audio_Chinese_pkg_version")
file_list=[]
download_list=[]

while True:
    file_json=pkg.readline()
    if file_json == "":
        break
    # print (file_json)
    file_list.append(file_json)
pkg.close()

all_file=open("all_file_list",'w')
for file_json in file_list:
    json_list=json.loads(file_json)
    file_name=json_list['remoteName']
    file_hash=json_list['md5']
    all_file.write(file_name+'\n')
    if verify_file(file_name,file_hash):
        print("No need to download "+file_name)
    else:
        print("Download "+file_name)
        download_list.append(file_name)
all_file.close()

file_output_list=open("download_list",'w')
for file_name in download_list:
    file_output_list.write(file_name+'\n')
file_output_list.close()
