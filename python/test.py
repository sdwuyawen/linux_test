# -*- coding: utf-8 -*-
"""
Created on Thu Mar 12 22:27:57 2015

@author: paul2
"""
import os
import sys
import string
import json
import platform
import glob
import time
import stat


def stat_file(argv):
    "stat file"
    statinfo = os.stat(argv[1])
    print 'size =', statinfo.st_size
    print 'atime = ', time.ctime(statinfo.st_atime)
    print 'mtime = ', time.ctime(statinfo.st_mtime)
    print 'ctime = ', time.ctime(statinfo.st_ctime)
    if stat.S_ISDIR(statinfo.st_mode):           #判断是否路径
        print 'Directory. '
    else:
        print 'Non-directory.'

    if stat.S_ISREG(statinfo.st_mode):           #判断是否一般文件
        print 'Regular file.'
    elif stat.S_ISLNK(statinfo.st_mode):         #判断是否链接文件
        print 'Shortcut.'
    elif stat.S_ISSOCK(statinfo.st_mode):        #判断是否套接字文件    
        print 'Socket.'
    elif stat.S_ISFIFO(statinfo.st_mode):        #判断是否命名管道
        print 'Named pipe.'
    elif stat.S_ISBLK(statinfo.st_mode):         #判断是否块设备
        print 'Block special device.'
    elif stat.S_ISCHR(statinfo.st_mode):         #判断是否字符设置
        print 'Character special device.'    
    return

def list_jpegs(argv):
    "glob files"
    current_dir = os.getcwd()
    os.chdir(argv[1])
    files = glob.glob('*.jpg')
    os.chdir(current_dir)
    for filename in files:
        print filename
    return

def write_json(filename, objs):
    """
    write json to file
    """
    fobj = open(filename, "w")
    encode_json = json.dumps(objs, sort_keys=True)
    fobj.write(encode_json)
    fobj.close()
    return

def read_json(argv):
    """
    read file json data
    """
    fobj = open(argv[1], 'r')
    decode_json = json.loads(fobj.read()) 
    # print type(decode_json)
    print '========================================='
    print 'number of objs = ', len(decode_json)
    print '========================================='
    for json_obj in decode_json:
        # print type(json_obj)
        for key in json_obj:
            print "%s = %s" % (key, json_obj[key])

    fobj.close()        
    return

def test_json():
    """
    test json
    """
    obj = [[1, 2, 3], 123, 123.123, 'abc', {'key1':(1, 2, 3), 'key2':(4, 5, 6)}]
    encodedjson = json.dumps(obj)
    print repr(obj)
    print encodedjson

    decodejson = json.loads(encodedjson)
    print type(decodejson)
    print decodejson[4]['key1']
    print decodejson    
    return

def list_file(argv):
    """
    list file
    """
    files = os.listdir(argv[1])
    for filename in files:
        print filename

    return

def main(argv):
    """
    main entry
    """
    argc = len(argv)
    print "argc = %d\n" % argc

    fobj = open(argv[1], 'r')
    dict1 = {}
    for line in fobj.readlines():
        line = line.rsplit('\n')
        left, right = line[0].split(':')
        print left, right        
        dict1[left] = right
    dirs = os.listdir(argv[2])
    type1 = 0
    for dir1 in dirs:
        for key in dict1:
            print key, dict1[key]
            if key == dir1:
                type1 = string.atoi(dict1[key], 10)

    print "type = %d" % type1           
    fobj.close()

if __name__ == '__main__':
    print platform.system()
    # main(sys.argv)  
    # list_file(sys.argv)
    # test_json()
    # read_json(sys.argv)
    # list_jpegs(sys.argv)
    stat_file(sys.argv)
