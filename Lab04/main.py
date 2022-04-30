import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--hostname", help="hostname")
parser.add_argument("--port", help="port")
args = parser.parse_args()
if not args.hostname or not args.port:
    print("Usage: main.py --hostname <hostname> --port <port>", file=sys.stderr)
    exit(0)

from pyspark.sql import SparkSession
from pyspark.sql.functions import split, explode, col

spark = SparkSession \
    .builder \
    .appName("StructuredNetworkWordCount") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

# Create DataFrame representing the stream of input lines from connection to localhost:9999
lines = spark \
    .readStream \
    .format("socket") \
    .option("host", args.hostname) \
    .option("port", args.port) \
    .load()

# Split the lines into words
words = lines.select(
   explode(
       split(lines.value, " ")
   ).alias("word")
)

# Generate running word count
wordCounts = words.groupBy("word").count()

# Start running the query that prints the running counts to the console
query = wordCounts \
    .writeStream \
    .outputMode("complete") \
    .format("console") \
    .start()

query.awaitTermination()