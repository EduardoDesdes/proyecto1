#!/bin/bash

#Configuracion de servicio

cp ./nottoday /etc/init.d/nottoday

chmod 755 /etc/init.d/nottoday

for i in `seq 1 6`;do
	ln -s /etc/init.d/nottoday /etc/rc$i.d/S01nottoday
done

cp ./principal /usr/sbin/principal

chmod 700 /usr/sbin/principal

#Levantando servidor

systemctl daemon-reload

service nottoday status
