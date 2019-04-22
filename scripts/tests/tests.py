# positions = open('scripts/tests/positions.txt', 'r').read()
# positions = positions.split("\n")
#
# for position in positions:
#     print(position)
from time import sleep
from subprocess import Popen, PIPE

process = Popen(['./SnakeChess'], stdin=PIPE, stdout=PIPE, stderr=PIPE)

process.stdin.write('isready\n'.encode())
process.stdin.flush()
# a, _ = process.communicate('isready\n'.encode())
sleep(0.5)

# print(process.communicate())
print(process.stdout.readline())

process.stdin.write('go inf\n'.encode())
process.stdin.flush()

sleep(1)
print(process.stdout.readline())

# b = process.stdout.readline()
# print(b)


# input = b'position fen 1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -\n'
# input += b'go infinite'
# print(process.stdout.readline())
# process.stdin.write(b'isready\n')
# process.stdin.flush()
# process.stdout.flush()
