import time
import zmq
import base64

def producer():
    # setup socket
    context = zmq.Context()
    zmq_socket = context.socket(zmq.PUSH)
    zmq_socket.bind("tcp://127.0.0.1:5557")

    # Read file content
    f = open("/home/virus/Desktop/optimisation/test.ppm",'rb')
    bytes = bytearray(f.read())

    # Encode to send
    strng = base64.b64encode(bytes)
    print("Sending file over")
    print("\n\nEncoded message size: ",len(strng)) # 4194328 in my case
    zmq_socket.send(strng)
    f.close()

producer()