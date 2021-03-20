echo "[*] Recuperacion de Computador"

echo "[*] Listando archivos comprometidos"
FILES_NTD=`find /home -name '*.ntd'`
NFILES=`echo "$FILES_NTD" | wc -l`
echo "[*] Total de archivos comprometidos: "$NFILES
CONT=0
echo "[*] Iniciando recuperacion"
for FILE in $FILES_NTD #test
do
	FILE2=`echo $FILE | sed s/....$//g`
	#Desencriptando
	if openssl enc -aes-256-cbc -d -pass file:key.txt -in $FILE -out $FILE2 2>/dev/null
	then
		rm $FILE
		CONT=$[$CONT+1]
		echo "Recuperado [$CONT/$NFILES]"
	else
		echo "Error file: $FILE"
		rm $FILE2
	fi
done

echo "[*] Terminó la restauracion de archivos"

