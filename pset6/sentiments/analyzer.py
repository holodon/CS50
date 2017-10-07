import nltk


class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # initialize tokenizer
        self.tokenizer = nltk.tokenize.TweetTokenizer()

        # use sets
        self.wordsPos = set()
        self.wordsNeg = set()

        # parse positives from file
        file = open(positives, "r")
        for line in file:
            if line and not line.startswith(";"):
                self.wordsPos.add(line.strip())
        file.close()

        # parse negatives from file
        file = open(negatives, "r")
        for line in file:
            if line and not line.startswith(";"):
                self.wordsNeg.add(line.strip())
        file.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # initialize counter
        score = 0

        # tokenize the text
        tokens = self.tokenizer.tokenize(text)

        # parse and analyze the text
        for word in tokens:
            word = word.lower()
            if word in self.wordsPos:
                score += 1
            elif word in self.wordsNeg:
                score -= 1

        # return result
        return score
