#!/bin/sh

if dpkg -l | grep -q "^ii\s*git"; then
    echo "[\e[1;92m+\e[0m] Git is already installed."
else
    echo "[\e[1;91m-\e[0m] Git is not yet installed."
    echo "[\e[1;94m~\e[0m] Installing git..."
    apt install git -y
fi

if dpkg -l | grep -q "^ii\s*build-essential"; then
    echo "[\e[1;92m+\e[0m] Essential build tools are already installed."
else
    echo "[\e[1;91m-\e[0m] Essential build tools aren't installed yet."
    echo "[\e[1;94m~\e[0m] Installing build essentials..."
    apt install build-essential -y
fi

git clone https://github.com/nthnn/zhivo.git --depth 1
echo "[\e[1;94m~\e[0m] Building Zhivo..."

cd zhivo && chmod +x ./build.sh && ./build.sh
echo "[\e[1;92m+\e[0m] Zhivo was successfully compiled!"

cp dist/zhivo /bin/zhivo
echo "[\e[1;92m+\e[0m] Zhivo is now installed on your system!"

cd .. && rm -rf zhivo
echo "[\e[1;92m+\e[0m] Build clean done"
