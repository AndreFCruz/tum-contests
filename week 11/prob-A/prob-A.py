"""
Week 11 - Problem A - Contact List
- Trie
"""

import sys
from queue import Queue


class TrieNode:
  def __init__(self, value, is_word_end: bool = False):
    self.value = value
    self.is_word_end = is_word_end
    self.children = list()
  
  def to_string(self, accumulated=''):
    ret = list()
    if self.is_word_end:
      ret.append(accumulated + self.value)

    for c in self.children:
      ret.extend(c.to_string(accumulated + self.value))
    return ret


def trie_insert(root: TrieNode, value):
  '''
  Inserts value in the given Trie,
  returns True if possible or False if repeated
  '''
  node = root
  for i in range(len(value)):
    new_node = None
    for child in node.children:
      if child.value == value[i]:
        new_node = child
        break

    if new_node is None:
      new_node = TrieNode(value[i])
      node.children.append(new_node)
    node = new_node

  if node.is_word_end: return False # repeated word
  node.is_word_end = True
  return True # successful insertion


def solve_test_case():
  n = int(input())
  contacts = [sys.stdin.readline().strip() for _ in range(n)]

  trie_root = TrieNode('')
  for c in contacts:
    trie_insert(trie_root, c)
  
  # Count number of intermediate nodes corresponding to word endings
  count = 0
  q = Queue()
  q.put_nowait(trie_root)
  while not q.empty():
    node = q.get_nowait()
    for c in node.children:
      q.put_nowait(c)
    
    if len(node.children) > 0 and node.is_word_end:
      count += 1

  return count


if __name__ == '__main__':
  n_cases = int(input())
  for i in range(n_cases):
      print('Case #{}: {}'.format(i + 1, solve_test_case()))
      sys.stdin.readline()
