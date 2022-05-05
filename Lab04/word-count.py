import sys
import argparse

"""
Recebe o hosts como argumento
"""
parser = argparse.ArgumentParser()
parser.add_argument("--hosts", help="hostname")
args = parser.parse_args()
"""
Confere se o hosts foi enviado como argumento
"""
if not args.hosts:
    print("Usage: main.py --hosts <host1:port1,host2:port2,...>", file=sys.stderr)
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
list_host = [item.split(":") for item in list_host]
streams = [spark.readStream \
        .format("socket") \
        .option("host", value[0]) \
        .option("port", int(value[1])) \
        .load() for value in list_host]

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


"""
Depois que esse código for executado, a computação de streaming será iniciada em segundo plano.
O objeto de consulta é um identificador para essa consulta de streaming ativa e decidimos aguardar o término da consulta usando awaitTermination()
para evitar que o processo seja encerrado enquanto a consulta estiver ativa.
"""
query.awaitTermination()