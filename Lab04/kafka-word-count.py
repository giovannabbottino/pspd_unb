import sys
import argparse

"""
Recebe hosts e topics como argumento
"""
parser = argparse.ArgumentParser()
parser.add_argument("--hosts", help="hosts")
parser.add_argument("--topics", help="topics")
args = parser.parse_args()
"""
Confere se os hosts e topics foram enviados como argumento
"""
if not args.hosts or not args.topics:
    print("Usage: main.py --hosts <host1:port1,host2:port2> --topics <topic1,topic2>", file=sys.stderr)
    exit(0)

"""
Cria uma sessão local com SparkSession
O ponto de entrada para a programação do Spark com a API Dataset e DataFrame.

Em ambientes em que isso foi criado antecipadamente, use o construtor para obter uma sessão existente:
"""
 
from pyspark.sql import SparkSession
spark = SparkSession \
    .builder \
    .appName("StructuredNetworkWordCount") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

"""
Cria um DataFrame de streaming que representa os dados de texto recebidos de um servidor escutando em 
args.hostname:args.port e transformar o DataFrame para calcular a contagem de palavras.

readStream é Interface usada para carregar um conjunto de dados de streaming de sistemas de armazenamento externos
format Especifica o formato da fonte de dados de entrada.

Esta linha DataFrame representa uma tabela ilimitada contendo os dados de texto de streaming.
Essa tabela contém uma coluna de strings denominada "valor” e cada linha nos dados de texto de streaming se torna uma linha na tabela. 
Observe que isso não está recebendo nenhum dado no momento, pois estamos apenas configurando a transformação e ainda não a iniciamos. 
"""

list_host = args.hosts.split(",")

list_topics = args.topics.split(",")

streams = [spark \
  .readStream  \
  .format("kafka") \
  .option("kafka.bootstrap.servers", host) \
  .option("subscribe", list_topics[i]) \
  .load() for i, host in enumerate(list_host)]

lines = streams[0]
for i in range(1, len(streams)):
    lines = lines.union(streams[i])

"""
Em seguida, usamos duas funções SQL embutidas - split e explode, para dividir cada linha em várias linhas com uma palavra cada. 
Além disso, usamos o alias da função para nomear a nova coluna como "word”. 
"""
from pyspark.sql.functions import split, explode

words = lines.select(
   explode(
       split(lines.value, " ")
   ).alias("word")
)

"""
Por fim, definimos o WordCounts DataFrame agrupando pelos valores únicos no Dataset e contando-os. 
Observe que este é um DataFrame de streaming que representa as contagens de palavras em execução do fluxo.
"""
wordCounts = words.groupBy("word").count()

schema = (words.groupBy().count()).selectExpr("cast (count as string) total")

"""
Agora configuramos a consulta nos dados de streaming. 
Tudo o que resta é realmente começar a receber dados e calcular as contagens. 
Para fazer isso, configuramos para imprimir o conjunto completo de contagens (especificado por outputMode("complete")) no console toda vez que eles são atualizados. 
E, em seguida, inicie a computação de streaming usando start().
"""
query = wordCounts \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()

count = schema \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()




from pyspark.sql.functions import lower, col, length

p = words.filter(lower(col("word").substr(1, 1)) == "p").groupBy().count().selectExpr("cast (count as string) p")
count_p = p \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()
s = words.filter(lower(col("word").substr(1, 1)) == "s").groupBy().count().selectExpr("cast (count as string) s")
count_s = s \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()
r = words.filter(lower(col("word").substr(1, 1)) == "r").groupBy().count().selectExpr("cast (count as string) r")
count_r = r \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()
p_6 =  words.filter(length("word") == 6).groupBy().count().selectExpr("cast (count as string) p6")
count_p_6 = p_6 \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()
p_8 =  words.filter(length("word") == 8).groupBy().count().selectExpr("cast (count as string) p8")
count_p_8 = p_8 \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()
p_11 =  words.filter(length("word") == 11).groupBy().count().selectExpr("cast (count as string) p11")
count_p_11 = p_11 \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()

"""
Depois que esse código for executado, a computação de streaming será iniciada em segundo plano.
O objeto de consulta é um identificador para essa consulta de streaming ativa e decidimos aguardar o término da consulta usando awaitTermination()
para evitar que o processo seja encerrado enquanto a consulta estiver ativa.
"""
query.awaitTermination()

count.awaitTermination()

count_p.awaitTermination()
count_s.awaitTermination()
count_r.awaitTermination()
count_p_6.awaitTermination()
count_p_8.awaitTermination()
count_p_11.awaitTermination()
