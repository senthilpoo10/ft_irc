#!/bin/bash

# Kill previous ircserv if any
pkill -f ircserv
sleep 1

PORT=6667
HOST=127.0.0.1

echo "🎯 Starting IRC test on $HOST:$PORT"

# Start the server
./ircserv $PORT pass &
SERVER_PID=$!
sleep 1

# Client A - Jennie (operator scenario)
(
sleep 1
echo -e "PASS pass\r"
sleep 1
echo -e "NICK Jennie\r"
sleep 1
echo -e "USER Jennie 0 * :Jennie\r"
sleep 1
echo -e "JOIN #test mypass\r"
sleep 1
echo -e "MODE #test +itlk mypass 3 Jennie\r"
sleep 1
echo -e "PRIVMSG #test :Hello from Jennie!\r"
sleep 1
echo -e "INVITE bob #test\r"
sleep 1
echo -e "KICK #test bob\r"
sleep 1
echo -e "PART #test\r"
) | nc $HOST $PORT > jennie_output.txt &

# Client B - bob (invited, joins and sends message)
(
sleep 3
echo -e "PASS pass\r"
sleep 1
echo -e "NICK bob\r"
sleep 1
echo -e "USER bob 0 * :Bob\r"
sleep 1
echo -e "JOIN #test mypass\r"
sleep 1
echo -e "PRIVMSG #test :Hi from Bob!\r"
sleep 1
echo -e "PART #test\r"
) | nc $HOST $PORT > bob_output.txt &

# Client C - noOp (non-operator trying to kick)
(
sleep 5
echo -e "PASS pass\r"
sleep 1
echo -e "NICK noOp\r"
sleep 1
echo -e "USER noop 0 * :No Operator\r"
sleep 1
echo -e "JOIN #test mypass\r"
sleep 1
echo -e "KICK #test Jennie\r" # Should fail due to lack of operator privileges
) | nc $HOST $PORT > noop_output.txt &

# Wait for all interactions to complete, then stop the server
sleep 10
kill $SERVER_PID
sleep 1

echo "📝 Test completed. Displaying outputs..."

# Show outputs from all clients
echo -e "\n=== Jennie Output ==="
cat jennie_output.txt

echo -e "\n=== Bob Output ==="
cat bob_output.txt

echo -e "\n=== NoOp Output ==="
cat noop_output.txt

echo -e "\n✅ Test finished. Run memory leak check using:"
echo "valgrind --leak-check=full ./ircserv $PORT pass"
