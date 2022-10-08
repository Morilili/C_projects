#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
  int distance;
  int mutually_liked_brands;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;

FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

/**
 * TODO: Complete this function
 * Creates and returns a user.
 */
User *create_user(char *name)
{
  // usernode
  User *new = NULL;
  new = (User *)calloc(1, sizeof(User));
  strcpy(new->name, name);
  new->friends = NULL;
  new->brands = NULL;
  new->visited = false;
  allUsers = insert_into_friend_list(allUsers, new);
  return new;
}

/**
 * TODO: Complete this function
 * Deletes a given user.
 */
// ***********************************************************************
int delete_user(User *user)
{

  if (in_friend_list(allUsers, user) == true) 
  { 
    allUsers = delete_from_friend_list(allUsers, user);
    FriendNode *p = allUsers;
    while (p != NULL)
    {
      if (in_friend_list(p->user->friends, user) == true)
      {
        p->user->friends = delete_from_friend_list(p->user->friends, user);
      }
      p = p->next;
    }
    return 0;
  }
  return -1;
}

/**
 * TODO: Complete this function
 * Create a friendship between user and friend.
 */
int add_friend(User *user, User *friend)
{
  if (in_friend_list(user->friends, friend) == true || in_friend_list(friend->friends, user) == true) return -1;
  user->friends = insert_into_friend_list(user->friends, friend);
  friend->friends = insert_into_friend_list(friend->friends, user);
  return 0;
}

/**
 * TODO: Complete this function
 * Removes a friendship between user and friend.
 */
int remove_friend(User *user, User *friend)
{
  if (in_friend_list(user->friends, friend) == false || in_friend_list(friend->friends, user) == false) return -1;
  user->friends = delete_from_friend_list(user->friends, friend);
  friend->friends = delete_from_friend_list(friend->friends, user);
  return 0;
}

/**
 * TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 */
int follow_brand(User *user, char *brand_name)
{
  if (in_brand_list(user->brands, brand_name) == true) return -1;
  user->brands = insert_into_brand_list(user->brands, brand_name);
  return 0;
}

/**
 * TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 */
int unfollow_brand(User *user, char *brand_name)
{
  if (in_brand_list(user->brands, brand_name) == false) return -1;
  user->brands = delete_from_brand_list(user->brands, brand_name);
  return 0;
}
  /**
   * TODO: Complete this function
   * Return the number of mutual friends between two users.
   */

int get_mutual_friends(User * a, User * b)
{
  FriendNode *p = a->friends;
  FriendNode *q = b->friends;
  int mutuals = 0;
  while (p != NULL)
  {
    while (q != NULL)
    {
      if (strcmp(p->user->name, q->user->name) == 0) mutuals++;
      q = q->next;
    }
    p = p->next;
  }
  return mutuals;
}

/**
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.
 */

FriendNode *adduser(FriendNode *h, User *u)
{
  FriendNode *traversal = h;
  FriendNode *friend = (FriendNode *)calloc(1, sizeof(FriendNode));
  friend->user = u;
  friend->next = NULL;
  if (h == NULL) return friend;
  else
  {
    while (traversal->next != NULL)
    {
      traversal = traversal->next;
    }
    traversal = friend;
    return h;
  }
}
FriendNode *delhead(FriendNode *h)
{
  if (h == NULL) return NULL;
  FriendNode *ret = h->next;
  FriendNode *p = h;
  free(p);
  return ret;
}
int get_degrees_of_connection(User *a, User *b)
{
  
  FriendNode *p = allUsers;
  while (p != NULL){
     p->user->visited = false;
     p->user->distance = 0;
     p = p->next;
  }
  if (a == b) return 0;
  FriendNode *h = NULL;
  h = adduser(h, a);
  a->visited = true;
  while (h != NULL)
  {
    User *tmp = h->user;
    h = delhead(h);
    FriendNode *traversal = tmp->friends;
    while (traversal != NULL)
    {
      if (traversal->user->visited == false)
      {
        traversal->user->visited = true;
        h = adduser(h, traversal->user);
        traversal->user->distance = tmp->distance + 1;
        if(strcmp(traversal->user->name, b->name) == 0)
        {
          while (h != NULL)
          {
            FriendNode *q = h;
            h = h->next;
            free(q);
          }
          return traversal->user->distance;
        }
      }
      traversal = traversal->next;
    }
  }
  return -1;
  
}

/**
 * TODO: Complete this function
 * Marks two brands as similar.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
  int in1 = get_brand_index(brandNameA);
  int in2 = get_brand_index(brandNameB);
  if (in1 != -1 && in2 != -1)
  {
    brand_adjacency_matrix[in1][in2] = 1;
    brand_adjacency_matrix[in2][in1] = 1;
  }
}

/**
 * TODO: Complete this function
 * Marks two brands as not similar.
 */
void remove_similar_brands(char *brandNameA, char *brandNameB)
{
  int in1 = get_brand_index(brandNameA);
  int in2 = get_brand_index(brandNameB);
  if (in1 != -1 && in2 != -1)
  {
    brand_adjacency_matrix[in1][in2] = 0;
    brand_adjacency_matrix[in2][in1] = 0;
  }
}

/**
 * TODO: Complete this function
 * Returns a suggested friend for the given user.
 */
int get_num_of_mlbrands(BrandNode *a, BrandNode *b)
{
  int num_of_same = 0;
  BrandNode *p = a;
  BrandNode *q = b;
  while (p != NULL)
  {
    while(q != NULL)
    {
      if (strcmp(p->brand_name, q->brand_name) == 0)
      {
        num_of_same++;
      }
      q = q->next;
    }
    q = b;
    p = p->next;
  }
  return num_of_same;
}

char *cmp_strbyalp(char *a, char *b)
{
  int len1 = strlen(a);
  int len2 = strlen(b);
  int len;
  if (len1 < len2) len = len1;
  else len = len2;
  for (int i = 0; i< len; i++)
  {
    int ascii1 = *(a + i);
    int ascii2 = *(b + i);
    if (ascii1 < ascii2) return b;
    else if (ascii1 > ascii2) return a;
  }
  if (len1<len2) return b;
  else return a;
}

User *get_suggested_friend(User *user)
{
  if (user == NULL) return NULL;
  // setting up the mutually liked brands value;
  FriendNode *travesal = allUsers;
  while (travesal != NULL)
  {
    travesal->user->mutually_liked_brands = 0;
    travesal = travesal->next;
  }

  int tracknumbrands = -1;
  User *holder = NULL;
  FriendNode *p = allUsers;
  while (p != NULL)
  {
    
    int mlbrands = get_num_of_mlbrands(user->brands, p->user->brands);
    p->user->mutually_liked_brands = mlbrands;
    if (strcmp(p->user->name, user->name) == 0) ;
    else if (mlbrands > tracknumbrands && in_friend_list(user->friends, p->user) == false)
    {
      tracknumbrands = mlbrands;
      holder = p->user;
    }
    else if (mlbrands == tracknumbrands && in_friend_list(user->friends, p->user) == false)
    {
      char name[MAX_STR_LEN];
      strcpy(name, cmp_strbyalp(holder->name, p->user->name));
      if (strcmp(name, p->user->name) == 0) holder = p->user;
    }
    p = p->next;
  }
  
  return holder;
}

/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 */
int add_suggested_friends(User *user, int n)
{
  int added = 0;
  for (int i = 0; i< n; i++)
  {
    User *tbadd = get_suggested_friend(user);
    if (tbadd == NULL)
    {
      break;
    }
    else
    {
      added++;
      add_friend(user, tbadd);
    }
  }
  return added;
}

/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 */
BrandNode *get_last(BrandNode *head)
{
  BrandNode *p = head;

  while (p->next != NULL)
  {
    p = p->next;
  }

  return p;
}

BrandNode *delete(BrandNode *head)
{
  BrandNode *p = head;

  while (p->next != NULL)
  {
    p = p->next;
  }
  free(p);
  return head;
}

int follow_suggested_brands(User *user, int n)
{
  
  int added = 0;
  BrandNode *sim_brand_list = NULL; // new brand linked list to keep track of all similar brands
  BrandNode *non_sim_list = NULL; // new brand linked list with non simlar brands
  BrandNode *traversal = user->brands;
  
  while (traversal != NULL)
  {
    int index = get_brand_index(traversal->brand_name);
    
    for (int i = 0; i < MAT_SIZE; i++)
    {
      int similar = brand_adjacency_matrix[index][i];
      
      if (similar == 0)
      {
        if (in_brand_list(non_sim_list, traversal->brand_name) == false)
          non_sim_list = insert_into_brand_list(non_sim_list, brand_names[i]);
      }
      else{
        if (in_brand_list(sim_brand_list, traversal->brand_name) == false)
          sim_brand_list = insert_into_brand_list(sim_brand_list, brand_names[i]);
      }
    }
    traversal = traversal->next;
  }

  for (int x = 0; x < n; x++)
  {
    if (sim_brand_list == NULL && non_sim_list == NULL) break;
    else if (sim_brand_list != NULL){
      BrandNode *p = get_last(sim_brand_list);
      if (in_brand_list(user->brands, p->brand_name) == false)
      {
        user->brands = insert_into_brand_list(user->brands, p->brand_name);
        added++;
      }
      sim_brand_list = delete (sim_brand_list);
    }
    else if (non_sim_list != NULL){
      BrandNode *p = get_last(non_sim_list);
      if (in_brand_list(user->brands, p->brand_name) == false)
      {
        user->brands = insert_into_brand_list(user->brands, p->brand_name);
        added++;
      }
    }
  }
  return added;
}
