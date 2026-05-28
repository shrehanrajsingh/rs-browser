#if !defined(EQUINOX_NODE_H)
#define EQUINOX_NODE_H

#include "header.hpp"
#include "ndata.hpp"

namespace equinox
{
/**
 * @brief Node in a linked structure with parent/child relationships.
 *
 * Stores a pointer to the next node in a list, a pointer to the parent
 * node, associated data, and a collection of child nodes.
 */
class Node
{
public:
  Node *next;
  Node *parent;
  NodeData nd;

  std::vector<Node *> children;

  /**
   * @brief Construct an empty node with no links.
   */
  Node () : next{ nullptr }, parent{ nullptr } {}
  /**
   * @brief Construct a node (next pointer initialized to nullptr).
   * @param _Next Unused parameter (kept for API compatibility).
   */
  Node (Node *_Next) : next{ nullptr }, parent{ nullptr } {}

  /**
   * @brief Link one node after another in the list.
   * @param _Head Node to link from.
   * @param _Next Node to link as next.
   */
  static void add_next (Node *, Node *);

  /**
   * @brief Print the node data and optionally its parent.
   * @param _Print_Parent Whether to include parent information.
   */
  void print (bool _Print_Parent = false);
  /**
   * @brief Get the text content for this node.
   * @return Text content string.
   */
  std::string get_text ();

  /**
   * @brief Destroy the node and its next chain.
   */
  ~Node ()
  {
    if (next != nullptr)
      delete next;
  }
};
} // namespace equinox

#endif // EQUINOX_NODE_H
