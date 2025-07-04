import socket
import json

SERVER_IP = "127.0.0.1"
SERVER_PORT = 4250
MAX_BYTES_TO_RECV = 1024
LOGIN_REQ_CODE = 101
SIGNUP_REQ_CODE = 102
TYPE_CODE_LEN = 1
DATA_SIZE_LEN = 4
MSB_to_LSB = 'big' # ordered from MSB to LSB
LSB_to_MSB = 'little' # ordered from LSB to MSB
UTF8_ENCODING = 'utf-8'


def send_login_req(sock, username, password):
    """
    This function sends a login request to the server.
    :param sock: the conversation socket created with the server.
    :type sock: socket.socket.
    :return: none.
    """
    json_data = {"username": username, "password": password} # building the JSON for the login request
    json_data_str = json.dumps(json_data) # converting the JSON to a string type
    data_size = len(json_data_str) # the len of it from

    msg = bytes([LOGIN_REQ_CODE])  # first put the Request code for identifying purposes
    msg = msg + data_size.to_bytes(DATA_SIZE_LEN, byteorder=LSB_to_MSB) # to_bytes(length, byteorder, signed=False)
    msg = msg + json_data_str.encode(UTF8_ENCODING) # Encoding using utf-8 encoding and adding it to the message

    sock.sendall(msg) # send the msg request ( in bytes )


def send_signup_req(sock, username, password, email):
    """
    This function sends a signup request to the server.
    :param sock: the conversation socket created with the server.
    :type sock: socket.socket.
    :return: none.
    """
    json_data = {"username": username, "password": password, "email": email} # building the JSON for the signup request
    json_data_str = json.dumps(json_data) # converting the JSON to a string type
    data_size = len(json_data_str) # the len of it from

    msg = bytes([LOGIN_REQ_CODE])  # first put the Request code for identifying purposes
    msg = msg + data_size.to_bytes(DATA_SIZE_LEN, byteorder=LSB_to_MSB) # to_bytes(length, byteorder, signed=False)
    msg = msg + json_data_str.encode(UTF8_ENCODING) # Encoding using utf-8 encoding and adding it to the message

    sock.sendall(msg) # send the msg request ( in bytes )


def receive_ans_size(sock):
    """
    This function receives a message data size from the server and casts it to 'int'.
    :param sock: the conversation socket created with the server.
    :type sock: socket.socket.
    :return: the message data size sent from the server.
    :rtype: str.
    """
    msg = sock.recv(DATA_SIZE_LEN)

    return int.from_bytes(msg, byteorder=MSB_to_LSB, signed=False)


def receive_answer(sock, data_size):
    """
    This function receives a message data from the server and casts it to 'str'.
    :param sock: the conversation socket created with the server.
    :param data_size: the number of bytes to read from the socket.
    :type sock: socket.socket.
    :type data_size: int.
    :return: the message data sent from the server.
    :rtype: str.
    """
    msg = sock.recv(data_size)

    return msg.decode()


def send_data_to_server(sock, data):
    """
    This function sends data to the server in the appropriate format/body

    :param sock: The socket connection with the server.
    :type sock: socket
    :param data: The data to be sent to the server.
    :type data: str or list
    """
    sock.sendall(data.encode())


def contact_with_server(sock):
    """
    This function contacts with the server by sending a signup request, receiving
    a response, sending a login request and receiving a response for that as well.
    :param sock: the conversation socket created with the server.
    :type sock: socket.socket.
    :return: none.
    """

    try:
        send_signup_req(sock, "user1", "1234", "user1@gmail.com")
        server_ans_size = receive_ans_size(sock)
        server_ans = receive_answer(sock, server_ans_size)

        print(server_ans)

        send_login_req(sock, "user1", "12345")
        server_ans_size = receive_ans_size(sock)
        server_ans = receive_answer(sock, server_ans_size)

        print(server_ans)

        send_signup_req(sock, "user3", "9ABC", "user3@gmail.com")
        server_ans_size = receive_ans_size(sock)
        server_ans = receive_answer(sock, server_ans_size)

        print(server_ans)

    except Exception as error:
        print("Error Here: ", error)


def main():
    # Create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        # Connecting to remote computer 4250
        server_address = (SERVER_IP, SERVER_PORT)
        try:
            sock.connect(server_address) # connect to the server
        except Exception as error:
            print("Error: ", error)

        contact_with_server(sock)


if __name__ == "__main__":
    main()
