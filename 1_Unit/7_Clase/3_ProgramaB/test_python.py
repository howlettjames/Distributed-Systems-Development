import math

MAX = 50000000
i = 0
seno = 0
coseno = 0
tangente = 0
loga = 0
raiz = 0

for i in range(MAX):
	seno += math.sin(i)
	coseno += math.cos(i)
	tangente += math.tan(i)
	loga += math.log(i + 1.0)
	raiz += math.sqrt(i)
	i+=1
