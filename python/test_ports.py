#coding:utf-8
import socket
import threading
import time

class SkPort(threading.Thread):
    def __init__(self,ip,port):
        threading.Thread.__init__(self)
        self.ip = ip
        self.port = port
    def run(self):
        sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sk.settimeout(1000)
        try:
            sk.connect((self.ip,self.port))
            print('Server %s port %d OK!' % (self.ip,self.port))
        except Exception:
            # print('Server %s port %d is not connected!' % (self.ip,self.port))
            error = self.port
        sk.close()
def main():
    ip = '127.0.0.1'
    sport = int(raw_input('begin port:'))
    eport = int(raw_input('end port:'))
    for port in range(sport,eport+1):
        item = (ip,port)
        t = SkPort(ip,port)
        t.start()
if __name__ == '__main__':
    main()
