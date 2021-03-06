"""
Week 11 - Problem B - J&J's
- Segment Tree
"""

import sys

# Segment Tree implementation
# From: https://www.geeksforgeeks.org/lazy-propagation-in-segment-tree-set-2/
MAX = 10 ** 7 + 1
tree = [0] * MAX  # To store segment tree
lazy = [0] * MAX  # To store pending updates


""" si -> index of current node in segment tree 
	ss and se -> Starting and ending indexes of elements 
				for which current nodes stores sum. 
	us and ue -> starting and ending indexes of update query 
	diff -> which we need to add in the range us to ue """
def updateRangeUtil(si, ss, se, us, ue, diff) : 

  # If lazy value is non-zero for current node 
  # of segment tree, then there are some 
  # pending updates. So we need to make sure 
  # that the pending updates are done before 
  # making new updates. Because this value may be 
  # used by parent after recursive calls 
  # (See last line of this function) 
  if (lazy[si] != 0) : 
    
    # Make pending updates using value 
    # stored in lazy nodes 
    tree[si] += (se - ss + 1) * lazy[si]

    # checking if it is not leaf node because if 
    # it is leaf node then we cannot go further 
    if (ss != se) : 
    
      # We can postpone updating children we don't 
      # need their new values now. 
      # Since we are not yet updating children of si, 
      # we need to set lazy flags for the children 
      lazy[si * 2 + 1] += lazy[si]
      lazy[si * 2 + 2] += lazy[si]
    
    # Set the lazy value for current node 
    # as 0 as it has been updated 
    lazy[si] = 0

  # out of range 
  if (ss > se or ss > ue or se < us) : 
    return 

  # Current segment is fully in range 
  if (ss >= us and se <= ue) : 
    
    # Add the difference to current node 
    tree[si] += (se - ss + 1) * diff

    # same logic for checking leaf node or not 
    if (ss != se) : 
    
      # This is where we store values in lazy nodes, 
      # rather than updating the segment tree itelf 
      # Since we don't need these updated values now 
      # we postpone updates by storing values in lazy[] 
      lazy[si * 2 + 1] += diff
      lazy[si * 2 + 2] += diff
    
    return

  # If not completely in rang, but overlaps, 
  # recur for children, 
  mid = (ss + se) // 2
  updateRangeUtil(si * 2 + 1, ss, mid, us, ue, diff)
  updateRangeUtil(si * 2 + 2, mid + 1, se, us, ue, diff)

  # And use the result of children calls
  # to update this node
  tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2]


# Function to update a range of values 
# in segment tree 
''' us and eu -> starting and ending indexes 
				of update query 
	ue -> ending index of update query 
	diff -> which we need to add in the range us to ue '''
def updateRange(n, us, ue, diff) : 
	updateRangeUtil(0, 0, n - 1, us, ue, diff)


''' A recursive function to get the sum of values 
	in given range of the array. The following are 
	parameters for this function. 
	si --> Index of current node in the segment tree. 
		Initially 0 is passed as root is always at' 
		index 0 
	ss & se --> Starting and ending indexes of the 
				segment represented by current node, 
				i.e., tree[si] 
	qs & qe --> Starting and ending indexes of query 
				range '''
def getSumUtil(ss, se, qs, qe, si) : 

	# If lazy flag is set for current node 
	# of segment tree, then there are 
	# some pending updates. So we need to 
	# make sure that the pending updates are 
	# done before processing the sub sum query 
	if (lazy[si] != 0) : 
	
		# Make pending updates to this node. 
		# Note that this node represents sum of 
		# elements in arr[ss..se] and all these 
		# elements must be increased by lazy[si] 
		tree[si] += (se - ss + 1) * lazy[si]

		# checking if it is not leaf node because if 
		# it is leaf node then we cannot go further 
		if (ss != se) : 
		
			# Since we are not yet updating children os si, 
			# we need to set lazy values for the children 
			lazy[si * 2 + 1] += lazy[si]
			lazy[si * 2 + 2] += lazy[si]

		# unset the lazy value for current node 
		# as it has been updated 
		lazy[si] = 0

	# Out of range 
	if (ss > se or ss > qe or se < qs) : 
		return 0

	# At this point we are sure that 
	# pending lazy updates are done for 
	# current node. So we can return value 
	# (same as it was for query in our previous post) 

	# If this segment lies in range 
	if (ss >= qs and se <= qe) : 
		return tree[si]

	# If a part of this segment overlaps 
	# with the given range 
	mid = (ss + se) // 2
	return (getSumUtil(ss, mid, qs, qe, 2 * si + 1) +
			getSumUtil(mid + 1, se, qs, qe, 2 * si + 2))


# Return sum of elements in range from 
# index qs (query start) to qe (query end). 
# It mainly uses getSumUtil() 
def getSum(n, qs, qe) : 
	
	# Check for erroneous input values 
	if (qs < 0 or qe > n - 1 or qs > qe) : 
		print("Invalid Input")
		return -1

	return getSumUtil(0, n - 1, qs, qe, 0)

# A recursive function that constructs 
# Segment Tree for array[ss..se]. 
# si is index of current node in segment 
# tree st. 
def constructSTUtil(arr, ss, se, si) : 

	# out of range as ss can never be 
	# greater than se 
	if (ss > se) : 
		return 

	# If there is one element in array, 
	# store it in current node of 
	# segment tree and return 
	if (ss == se) : 
	
		tree[si] = arr[ss]
		return
	
	# If there are more than one elements, 
	# then recur for left and right subtrees 
	# and store the sum of values in this node 
	mid = (ss + se) // 2
	constructSTUtil(arr, ss, mid, si * 2 + 1)
	constructSTUtil(arr, mid + 1, se, si * 2 + 2)

	tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2]


''' Function to construct segment tree 
from given array. This function allocates memory 
for segment tree and calls constructSTUtil() 
to fill the allocated memory '''
def constructST(arr, n) : 
	# Fill the allocated memory st 
	constructSTUtil(arr, 0, n - 1, 0)


def solve_test_case():
  n_glasses, k_queries = map(int, sys.stdin.readline().rstrip().split(' '))
  queries = [sys.stdin.readline().rstrip().split(' ') for _ in range(k_queries)]

  n_glasses += 1
  st = [0] * n_glasses  # segment tree
  constructST(st, n_glasses)

  answer = 0
  for q in queries:
    # import ipdb; ipdb.set_trace()
    if q[0] == 'q':
      target_glass = int(q[1])
      answer += getSum(n_glasses, target_glass, target_glass)
    elif q[0] == 'i':
      l, r, v = map(int, q[1:])
      updateRange(n_glasses, l, r, v)
    else:
      # raise ValueError('Invalid argument for query "{}"'.format(q[0]))
      print('Invalid argument for query "{}"'.format(q[0]))

  return answer % 1000000007


if __name__ == '__main__':
  n_cases = int(input())
  for i in range(n_cases):
      print('Case #{}: {}'.format(i + 1, solve_test_case()))
      sys.stdin.readline()
