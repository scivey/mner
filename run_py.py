import sys
from pprint import pprint
sys.path.append('./src/gen-py')
from NerTagger import NerTagger
from thrift import Thrift
from thrift.protocol import TBinaryProtocol
from thrift.transport import TSocket, TTransport

def main():
    sock = TSocket.TSocket('localhost', 8089)
    transport = TTransport.TBufferedTransport(sock)
    protocol = TBinaryProtocol.TBinaryProtocol(transport)
    client = NerTagger.Client(protocol)
    transport.open()
    result = client.tag("Barack Obama is the president of the United States of America.")
    pprint(result)

if __name__ == '__main__':
    main()
