#!/bin/bash

#Compilando el server
gcc src/server.c -o server

#Creando directorio donde se guardaran los archivos enviados
mkdir /tmp/enctest

#Configuracion de servicio

cp Daemon/server /etc/init.d/server

chmod 755 /etc/init.d/server

for i in `seq 1 6`;do
	ln -s /etc/init.d/server /etc/rc$i.d/S01server
done

#Levantando servidor

systemctl daemon-reload

service nottoday start
service nottoday status

#Enviando los archivos a /opt/server
mkdir /opt/server/
cp -r * /opt/server

ln -s /opt/server/adminpanel/colmena.sh /usr/bin/colmena
