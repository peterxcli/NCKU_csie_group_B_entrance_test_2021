import os
import zmq
import json
import numpy as np
import math

class MessageBuffer:
    def __init__(self, word, index, total_num):
        self.word = word
        self.index = index
        self.total_num = total_num
        self.src_word = ""
    
    def set_src_word(self, word):
        self.src_word = word

    def to_dict(self):
        msg = {
            "src_word": self.src_word,
            "word": self.word,
            "index": self.index,
            "total_buffers_num": self.total_num
        }
        return msg

def split_word(word, num):
    buffers = []
    hop_size = math.ceil(len(word) / num)

    idx = 0
    for n in range(num):
        w = word[idx: idx+hop_size]

        buffers.append( 
            MessageBuffer(word=w, index=n, total_num=num)
        )

        idx = idx + hop_size
    
    return buffers

def connect_to_server(context, para):
    socket = context.socket(zmq.REQ)
    socket.connect(para["socket_system_server"])
    return socket

def connect_to_consumer(context, para):
    socket = context.socket(zmq.PUSH)
    socket.bind(para["socket_producer_consumer"])
    return socket

def recv_word(socket):
    return socket.recv_string()

def send_start_signal(socket, para):
    socket.send_string(f"{para['userID']} s")

def send_buffer(socket, buf):
    msg = json.dumps(buf.to_dict())
    socket.send_string(msg)

def main(para: dict):
    context = zmq.Context()
    socket_consumer = connect_to_consumer(context, para)
    socket_server = connect_to_server(context, para)
    
    # receive word from server
    send_start_signal(socket_server, para)
    word = recv_word(socket_server)
    # prepare buffers for sending to consumer
    msg_buffers = split_word(word, para["num_to_split"])
    for msg in msg_buffers:
        msg.set_src_word(word)

    # send to consumers
    while len(msg_buffers) > 0:
        buf = msg_buffers.pop()
        send_buffer(socket_consumer, buf)

    print("[Producer] finish sending all message buffer")

    socket_server.close()
    socket_consumer.close()

if __name__ == "__main__":

    # read default parameters setting
    with open("./SystemParameters.json", "r") as f:
        para = json.load(f)
    # 
    main(para)

