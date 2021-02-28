import datetime, time
ti = datetime.datetime(2021, 2, 26, 2, 00, 00)
tf = datetime.datetime(2021, 2, 26, 2, 20, 00)
print("Tiempos")
print("ti =",round(time.mktime(ti.timetuple())))
print("tf =",round(time.mktime(tf.timetuple())))
