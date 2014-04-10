#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 10 14:11:04 2014

@author: paul
"""
import sys
from optparse import OptionParser

options = OptionParser(usage='%prog string [options]', description='hexdump input string')
options.add_option('-p', '--port', type='int', default=443, help='TCP port to test (default: 443)')
options.add_option('-s', '--starttls', action='store_true', default=False, help='Check STARTTLS')
options.add_option('-d', '--debug', action='store_true', default=False, help='Enable debug output')

def hexdump(s):
    for b in xrange(0, len(s), 16):
        lin = [c for c in s[b : b + 16]]
        hxdat = ' '.join('%02X' % ord(c) for c in lin)
        pdat = ''.join((c if 32 <= ord(c) <= 126 else '.' )for c in lin)
        print '  %04x: %-48s %s' % (b, hxdat, pdat)
    print

def h2bin(x):
    return x.replace(' ', '').replace('\n', '').decode('hex')

hb = h2bin(''' 
18 03 02 00 03
01 40 00
''')
    
def main():
    opts, args = options.parse_args()
    if len(args) < 1:
        options.print_help()
        return
        
    print '============[options]============='    
    print 'port=%d' % (opts.port)
    print 'starttls=%d' % (opts.starttls)
    print 'debug=%d' % (opts.debug)
    print
    
    print '=================================='    
    print 'hexdump test:'    
    hexdump(args[0])        
    sys.stdout.flush()
    
    print '=================================='    
    print 'h2bin test:'
    hxdat = ' '.join('%02X' % ord(c) for c in hb)
    print '  %s' % (hxdat)
    print
    
if __name__ == '__main__':
    main()