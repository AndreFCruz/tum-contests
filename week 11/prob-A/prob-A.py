"""
Week 11 - Problem A - Contact List
"""

import sys


class TrieNode:
  def __init__(self, value, is_word_end: bool = False):
    self.value = value
    self.is_word_end = is_word_end
    self.children = list()

def trie_search(root: TrieNode, value):
  trie_search_aux(root, value, '')

## NOTE This recursive implementation can be replaced with Iteration + Queue + backtracking
def trie_search_aux(root: TrieNode, value, accumulated):
  if value == accumulated:
    return root

  for child in root.children:
    res = trie_search_aux(child, value, accumulated + child.value)
    if res is not None:
      return res

  return None

def trie_insert(root: TrieNode, value):
  pass

def solve_test_case():



if __name__ == '__main__':
  n_cases = int(input())
  for i in range(n_cases):
      print('Case #{}: {}'.format(i + 1, solve_test_case()))
      sys.stdin.readline()
