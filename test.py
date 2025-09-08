import socket
import threading
import time

HOST = "127.0.0.1"
PORT = 6667
CHANNEL = "#42"
PASS = "pass"

def irc_client(nick, messages):
    """Basit IRC client fonksiyonu"""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    # Kayıt
    s.sendall(f"PASS {PASS}\r\n".encode())
    s.sendall(f"NICK {nick}\r\n".encode())
    s.sendall(f"USER {nick} 0 * :{nick}\r\n".encode())
    time.sleep(1)
    s.sendall(f"JOIN {CHANNEL}\r\n".encode())

    # Okuma thread’i başlat
    def reader():
        while True:
            try:
                data = s.recv(4096)
                if not data:
                    break
                print(f"[{nick}] {data.decode(errors='ignore').strip()}")
            except Exception as e:
                print(f"[{nick}] error: {e}")
                break

    threading.Thread(target=reader, daemon=True).start()

    # Mesaj gönderme
    for msg in messages:
        time.sleep(2)
        s.sendall(f"PRIVMSG {CHANNEL} :{msg}\r\n".encode())

    # Özel mesaj örneği
    if nick == "client1":
        time.sleep(3)
        s.sendall("PRIVMSG client2 :Selam client2!\r\n".encode())

    time.sleep(10)
    s.close()


if __name__ == "__main__":
    clients = [
        ("client1", ["Merhaba ben client1"]),
        ("client2", ["Selam burası client2"]),
        ("client3", ["Hey millet ben geldim"]),
    ]

    threads = []
    for nick, msgs in clients:
        t = threading.Thread(target=irc_client, args=(nick, msgs))
        t.start()
        threads.append(t)

    for t in threads:
        t.join()
