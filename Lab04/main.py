from __future__ import print_function

import sys

from pyspark import SparkContext
from pyspark.streaming import StreamingContext

if __name__ == "__main__": 
    print(sys.argv)
    if len(sys.argv) < 3:
        print("Usage: main.py <hostname> <port>", file=sys.stderr)
        exit(0)
    sc = SparkContext("local[2]", "NetworkWordCount")
    ssc = StreamingContext(sc, 1)

    lines = ssc.socketTextStream("localhost", 9999)

    words = lines.flatMap(lambda line: line.split(" "))
    counts = words.map(lambda word: (word, 1))\
                  .reduceByKey(lambda a, b: a+b)
    counts.pprint()

    ssc.start()
    ssc.awaitTermination()