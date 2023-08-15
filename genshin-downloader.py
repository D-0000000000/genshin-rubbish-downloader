import os
import json
import hashlib

download_url="https://autopatchcn.yuanshen.com/client_app/download/pc_zip/20230625120029_C0NLGkC0fxSaNKnu/ScatteredFiles/"
pkg_name=[]
num_thread=1

ab_list=[]

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

def parse_config():
    fin=open("config.json")
    cfg_file=fin.read()
    fin.close()
    cfg_json=json.loads(cfg_file)
    global download_url
    download_url=cfg_json['url']
    global pkg_name
    pkg_name=cfg_json['pkg']
    global num_thread
    num_thread=cfg_json['thread']
    print(pkg_name)

def download_package():
    print(pkg_name)
    for i in pkg_name:
        os.system("curl -O "+download_url+i)

def read_pkg(package_name):
    fin=open(package_name)
    while True:
        file_json=fin.readline()
        if file_json == "":
            break
        # print (file_json)
        ab_list.append(file_json)
    fin.close()

# os.system("curl -O "+download_url+"pkg_version")
# pkg=open("pkg_version")
# pkg=open("Audio_Chinese_pkg_version")
file_list=[]
download_list=[]

parse_config()
download_package()
for i in pkg_name:
    read_pkg(i)

all_file=open("all_file_list",'w')
for file_json in ab_list:
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

print("Start download")
os.system("./downloader.out download all_file_list "+str(num_thread)+" "+download_url)
print("Clean old files")
os.system("./downloader.out clean all_file_list")

