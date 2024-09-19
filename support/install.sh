#!/bin/sh

if dpkg -l | grep -q "^ii\s*git"; then
    echo "[+] Git is already installed."
else
    echo "[-] Git is not installed."
    echo "[*] Installing git..."
    apt install git -y
fi

git clone https://github.com/nthnn/zhivo.git --depth 1
echo "[*] Building Zhivo..."

cd zhivo && chmod +x ./build.sh && ./build.sh
echo "[+] Zhivo was successfully compiled!"

cp dist/zhivo /bin/zhivo
echo "[+] Zhivo is now installed on your system!"

cd .. && rm -rf zhivo
echo "[+] Build clean done"
