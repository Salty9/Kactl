/**
 * Author: Idk
 * Description: Centeroid decompostion template
*/
vector<vector<int>> adj;
vector<bool> is_removed;
vector<int> subtree_size;

int get_subtree_size(int node, int parent = -1) {
  subtree_size[node] = 1;
  for (int child: adj[node]) {
    if (child == parent || is_removed[child]) {
      continue;
    }
    subtree_size[node] += get_subtree_size(child, node);
  }
  return subtree_size[node];
}
int get_centroid(int node, int tree_size, int parent = -1) {
  for (int child: adj[node]) {
    if (child == parent || is_removed[child]) {
      continue;
    }
    if (subtree_size[child] * 2 > tree_size) {
      return get_centroid(child, tree_size, node);
    }
  }
  return node;
}
void build_centroid_decomp(int node = 0) {
  int centroid = get_centroid(node, get_subtree_size(node));
  // do something
  is_removed[centroid] = true;
  for (int child: adj[centroid]) {
    if (is_removed[child]) {
      continue;
    }
    build_centroid_decomp(child);
  }
}