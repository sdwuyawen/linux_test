# -*- coding: utf-8 -*-
"""
Created on Thu Mar 12 22:27:57 2015

@author: paul2
"""
import sys
import sqlite3  

def main(argv):
    """
    main entry
    """
    argc = len(argv)

    db = sqlite3.connect('a.db')  
    c = db.cursor()

    c.execute('create table if not exists users(name text, level int)')
    db.commit() 

    if argc > 1:
        pass
    else:    
        c.execute('insert into users(name,level) values("tian", 9)')
        c.execute('insert into users(name,level) values("li", 2)')
        c.execute('insert into users(name,level) values("wu", 3)')
        db.commit()
    
    if argc > 1:
        c.execute('select * from users where name=\'%s\'' % argv[1])        
    else:    
        c.execute('select * from users where name=\'chen\'')
    print "\ntest all :"
    #if empty, return empty list
    print c.fetchall()
    c.execute('select * from users')
    print c.fetchall()

    c.execute('select * from users')
    print "\ntest one :"
    #if empty, return None
    while True:
        r = c.fetchone()
        print r            
        if not r:
            break

if __name__ == '__main__':
    main(sys.argv)
