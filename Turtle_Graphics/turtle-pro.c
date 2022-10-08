#include "imgUtils.c"


// This lets the driver code override the image size if it needs to. Make sure 
// you don't hard-code these values anywhere!
#ifndef SIZEX
  #define SIZEX 512
  #define SIZEY 512
#endif 

/*---------------------------------------------------------------------------*/

/**
 * This struct contains one node of the linked list, which represents a single 
 * command to the Turtle. It's field should include:
 * 
 *  - cmd   : A char array of size 10 holding the command name
 * 
 *  - val   : An integer that stores a parameter for the command (like forward,
 *            backward and colour). 
 * 
 *  - next  : A pointer to a struct of the same type, this is used for the 
 *            linked list
 * 
 * TODO: Complete this struct definition
 ****/

typedef struct cmdnode {
  char cmd[10];
  int val;
  struct cmdnode *next;
  struct cmdnode *loop_head;
} CmdNode;

/*---------------------------------------------------------------------------*/
void insertCommandLoop(CmdNode *lhead, CmdNode *new_CmdNode){
  CmdNode *p = NULL;
  p = lhead->loop_head;
  
  if (p == NULL)
  {
    lhead->loop_head = new_CmdNode;
    return;
  }

  while (p->next!=NULL)
  {
    p = p->next;
  }
  p->next = new_CmdNode;
}

CmdNode *newCmdNode(char cmd[10], int val) {
  /** 
   * This function allocates a new CmdNode struct and initializes it's values 
   * based on the input paramaters given. The next pointer is always 
   * initialized to NULL.
   * 
   * If the 'cmd' parameter is not a correct command, then print
   * "Invalid command.\n" and return NULL.
   * 
   * Note that we will always pass in a value here, even if the 
   * command doesn't need one. In this case, we can just ignore 
   * it. It saves us from having to make separate functions to 
   * deal with this.
   * 
   * TODO: Implement this function
   */
  if (strcmp(cmd,"penup") == 0||strcmp(cmd,"pendown") == 0||strcmp(cmd,"colour")==0 ||strcmp(cmd,"forward")==0||strcmp(cmd,"backward") == 0||strcmp(cmd,"right")==0||strcmp(cmd,"left")==0||strcmp(cmd,"loop")==0)
  {
    CmdNode *newNode = NULL;
    newNode = (CmdNode *)calloc(1, sizeof(CmdNode));
    strcpy(newNode->cmd, cmd);
    newNode->val = val;
    newNode->next = NULL;
    newNode->loop_head = NULL;
    return newNode;
  }
  printf("Invalid command.\n");
  return NULL; 
}

/*---------------------------------------------------------------------------*/

void printCommandList(CmdNode *head) {
  /**
   * This function prints out each command in the linked list one after the 
   * other. Each command MUST also have a line number printed before it, this 
   * is what you will be using to modify / delete them. To do this, initialize 
   * a counter and then increment it for each command.
   * 
   * Depending on whether or not the command needs an additional value 
   * (like forward, backward and colour), use one of the following statements 
   * to print out the information for each node: 
   *            [ The format is "linenumber: command value" ]
   * 
   * printf("%3d: %s %d\n", ... );     [With a value]
   * 
   * printf("%3d: %s\n", ... );        [Without a value]
   * 
   * Obviously, you also need to pass in the correct parameters to the print 
   * function yourself, this is just so you have the correct format.
   * 
   * TODO: Implement this function
   */
  CmdNode *p = NULL;
  int line_num = 0;
  p = head;
  CmdNode *loop = NULL;

  while (p != NULL)
  {
    if (strcmp(p->cmd, "colour") == 0||strcmp(p->cmd, "forward")==0||strcmp(p->cmd, "backward")==0)
    {
      printf("%3d: %s %d\n", line_num, p->cmd, p->val ); 
      line_num++;
    }
    else if (strcmp(p->cmd, "loop") == 0)
    {
      printf("%3d: %s %d\n", line_num, p->cmd, p->val ); 
      line_num++;
      loop = p->loop_head;
      while(loop != NULL)
      {
        if (strcmp(loop->cmd, "colour") == 0||strcmp(loop->cmd, "forward")==0||strcmp(loop->cmd, "backward")==0)
        {
          printf("%3d:   %s %d\n", line_num, loop->cmd, loop->val ); 
          line_num++;
        }
        else
        {
          printf("%3d:   %s\n", line_num, loop->cmd); 
          line_num++;
        }
        loop = loop->next;
      }
    }
    else
    {
      printf("%3d: %s\n", line_num, p->cmd); 
      line_num++;
    }
    
    p = p->next;
  }
}

/*---------------------------------------------------------------------------*/

void queryByCommand(CmdNode *head, char cmd[10]) {
  /**
   * This function looks for commands in the linked list that match the input 
   * query. It prints them out in the same format as the printCommandList() 
   * function.
   * 
   * Make sure that the line number of the commands that match is the same as 
   * the line number that would be printed by the printCommandList() function.
   * 
   * --------------------------------------------------------------------------
   * 
   * For instance, if your printCommandList function outputs
   * 
   *    0: penup
   *    1: forward 200
   *    2: right
   *    3: forward 50
   * 
   * Then, if this function is called with the same list and cmd = "forward", 
   * then your output here should be
   * 
   *    1: forward 200
   *    3: forward 50
   * 
   * TODO: Implement this function
   */
  CmdNode *p = NULL;
  int line_num = 0;
  p = head;
  CmdNode *loop = NULL;
  
  while (p != NULL)
  {
    if (strcmp(cmd, p->cmd) == 0)
    {
      if (strcmp(p->cmd, "colour") == 0||strcmp(p->cmd, "forward")==0||strcmp(p->cmd, "backward")==0||strcmp(p->cmd, "loop")==0)
      {
        printf("%3d: %s %d\n", line_num, p->cmd, p->val ); 
      }
      else
      {
        printf("%3d: %s\n", line_num, p->cmd); 
      }
    }
    if (p->loop_head != NULL)
    {
      loop = p->loop_head;
      line_num++;
      while (loop != NULL)
      {
        if (strcmp(cmd, loop->cmd) == 0)
        {
          if (strcmp(loop->cmd, "colour") == 0||strcmp(loop->cmd, "forward")==0||strcmp(loop->cmd, "backward")==0)
          {
            printf("%3d: %s %d\n", line_num, loop->cmd, loop->val ); 
          }
          else
          {
            printf("%3d: %s\n", line_num, loop->cmd); 
          }
        }
        line_num++;
        loop = loop->next;
      }
      line_num--;
    }
    line_num++;
    p = p->next;
  }
}

/*---------------------------------------------------------------------------*/

int countCommands(CmdNode *head) {
  /**
   * This function counts and returns the length of the linked list. It 
   * requires list traversal.
   * 
   * TODO: Implement this function
   */
  int len = 0;
  CmdNode *p = NULL;
  p = head;
  CmdNode *loop = NULL;

  while (p != NULL)
  {
    if (p->loop_head != NULL)
    {
      loop = p->loop_head;
      len++;
      while (loop != NULL)
      {
        len++;
        loop = loop->next;
      }
    }
    else
    {
      len++;
    }
    p = p->next;
  }

  return len;
}

/*---------------------------------------------------------------------------*/

CmdNode *insertCommand(CmdNode *head, CmdNode *new_CmdNode) {
  /**
   * This function inserts the node new_CmdNode *at the tail* of the linked 
   * list. (You are adding a command at the end).
   * 
   * If head == NULL, then the linked list is still empty.
   * 
   * It returns a pointer to the head of the linked list with the new node 
   * added into it.
   * 
   * TODO: Implement this function
   */
  CmdNode *p = NULL;
  p = head;
  if (p == NULL)
  {
    return new_CmdNode;
  }

  while (p->next != NULL)
  {
    p = p->next;
  }
  p->next = new_CmdNode;
  return head;
}

/*---------------------------------------------------------------------------*/

CmdNode *deleteCommandList(CmdNode *head) {
  /**
   * This function deletes the linked list of commands, releasing all the 
   * memory allocated to the nodes in the linked list.
   * 
   * Returns a NULL pointer so that the head of the list can be set to NULL
   * after deletion.
   * 
   * TODO: Implement this function
   */
  CmdNode *p = NULL;
  CmdNode *q = NULL;
  CmdNode *loop = NULL;
  p = head;
  while (p != NULL)
  {
    if (p->loop_head != NULL)
    {
      loop = p->loop_head;
      while (loop != NULL)
      {
        q = loop->next;
        free(loop);
        loop = q;
      }
    }

    q = p->next;
    free(p);
    p = q;
  }
  return p;
}

/*---------------------------------------------------------------------------*/
int checkxcoor(int xcoor)
{
  if (xcoor > SIZEX-1) return SIZEX - 1;
  if (xcoor < 0) return 0;
  else return xcoor;
}

int checkycoor(int ycoor)
{
  if (ycoor > SIZEY-1) return SIZEY - 1;
  if (ycoor < 0) return 0;
  else return ycoor;
}

int checkcolour(int col)
{
  if (col > 255) return 255;
  if (col < 0) return 0;
  else return col;
}

int mod(int **a, int b)
{
  return (((**a)%(b))+(b))%b;
}

void actualrun(Image *im, int *direction, int *draw, int *colour, int *xcoor, int *ycoor, char *cmd, CmdNode *p)
{
  
  if (strcmp(cmd, "penup") == 0)
    *draw = 1;
  if (strcmp(cmd, "pendown") == 0) *draw = 0;
  if (strcmp(cmd, "right") == 0) *(direction) += 1;
  if (strcmp(cmd, "left") == 0) *(direction) -= 1;
  if (strcmp(cmd, "colour") == 0)
  {
    *colour = checkcolour(p->val);
  }
  

  if (strcmp(cmd, "forward") == 0)
  {
    if (mod(&direction, 4) == 0)
    {
      int og = *xcoor;
      *xcoor = checkxcoor(*xcoor + p->val);
      if (*draw == 0) drawLine(im, og, *ycoor, *xcoor, *ycoor, *colour);
  
    }
    if (mod(&direction, 4) == 1)
    {
      int og = *ycoor;
      *ycoor = checkycoor(*ycoor + p->val);
      if (*draw == 0) drawLine(im, *xcoor, og, *xcoor, *ycoor, *colour);
    }
    if (mod(&direction, 4) == 2)
    {  
      int og = *xcoor;
      *xcoor = checkxcoor(*xcoor - p->val);
      if (*draw == 0) drawLine(im, og, *ycoor, *xcoor, *ycoor, *colour);
    }
    if (mod(&direction, 4) == 3)
    {
      int og = *ycoor;
      *ycoor = checkycoor(*ycoor - p->val);
      if (*draw == 0) drawLine(im, *xcoor, og, *xcoor, *ycoor, *colour);
    }
  }
  if (strcmp(cmd, "backward") == 0)
  {
    
    if (mod(&direction, 4) == 0)
    {
      int og = *xcoor;
      *xcoor = checkxcoor(*xcoor - p->val);
      if (*draw == 0) drawLine(im, og, *ycoor, *xcoor, *ycoor, *colour);

    }
    if (mod(&direction, 4) == 1)
    {
      int og = *ycoor;
      *ycoor = checkycoor(*ycoor - p->val);
      if (*draw == 0) drawLine(im, *xcoor, og, *xcoor, *ycoor, *colour);
    }
    if (mod(&direction, 4) == 2)
    {  
      int og = *xcoor;
      *xcoor = checkxcoor(*xcoor + p->val);
      if (*draw == 0) drawLine(im, og, *ycoor, *xcoor, *ycoor, *colour);
    }
    if (mod(&direction, 4) == 3)
    {
      int og = *ycoor;
      *ycoor = checkycoor(*ycoor + p->val);
      if (*draw == 0) drawLine(im, *xcoor, og, *xcoor, *ycoor, *colour);
    }
  }
}

void run(Image *im, CmdNode *head, int *endX, int *endY) {
  /**
   * This function runs the list of commands to move the turtle around and draw 
   * the image, and returns the final position of the turtle in the variables 
   * endX and endY.
   * 
   * --------------------------------------------------------------------------
   * 
   * NOTE: In the image we work with, the top-left pixel is (0,0),
   *       - x increases as you go right, up till SIZEX-1
   *       - y increases as you go down, up till SIZEY-1
   * 
   *                 (0,0)                 (SIZEX-1, 0)
   *                   x------------------------x
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |          IMAGE         |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   x------------------------x
   *             (0, SIZEY-1)            (SIZEX-1, SIZEY-1)
   * 
   * The image is in grayscale (black and white), so the colours are numbers 
   * from [0-255] where 0 is black, 255 is white, and the values in between 
   * are varying levels of gray.
   * 
   * You need to understand how the (x,y) values are stored so you know how 
   * they should be updated when you move (down means y increases, etc). You do 
   * not need to use the 'im' variable for anything other than passing it into 
   * the drawLine() function described below.
   * 
   * --------------------------------------------------------------------------
   * 
   * Here's the setup - There is a turtle (with a pen) that walks along the 
   * image. When the pen is down (on the paper), it draws a line along the path 
   * that it walks on. (If you want to play around with this, consider looking 
   * at the `turtle` library in python!)
   * 
   * The turtle initially starts at pixel (0, 0) [at the top left], 
   * facing right (in the positive x direction). The pen starts off
   * as `down`, with the default colour being black (0).
   * 
   * You need to go through the linked list and 'run' the commands to keep 
   * track of the turtles position, and draw the appropriate lines. Here is 
   * what each command should do:
   * 
   *  - penup          : Put the pen up (stop drawing)
   *  - pendown        : Put the pen down (start / continue drawing)
   *  - colour <val>   : Draw lines in colour <val> from now on
   *  - forward <val>  : Move the turtle forward <val> steps (pixels) 
   *                     in the direction it is facing.
   *  - backward <val> : Same as above, except move backwards
   *  - right          : Turn the turtle to the right by 90 degrees
   *  - left           : Turn the turtle to the left by 90 degrees
   * 
   * NOTE: Make sure that you do *not* go outside the image. For this, set the 
   * maximum and minimum values of x and y to be 0 and SIZEX-1 / SIZEY-1 
   * respectively.
   * 
   * For instance, if the turtle is at (0,0) facing right, and your command is
   * `forward 100000`, it will only go forward till (SIZEX-1, 0), and end 
   * up at the rightmost pixel in that row.
   * 
   * IMPORTANT: Once you are done with all the commands, make sure you save the 
   * final (x,y) location of the turtle into the variables endX and endY.
   * 
   * --------------------------------------------------------------------------
   * 
   * We have already implemented a drawLine() function (in imgUtils.c) which 
   * you should use to actually draw the lines. It takes in the following:
   * 
   *      - a pointer to an image struct (use 'im')
   *      - (x,y) location of start point
   *      - (x,y) location of end point
   *      - a colour to draw the line in [0-255]
   *          
   * Note that this function only draws horizontal and vertical lines, so 
   * either the x values or the y values of both points must be the same. 
   * Both these points *must* also be within the image. 
   *          [ 0 <= x < SIZEX,  0 <= y < SIZEY ]
   * 
   * 
   * TODO: Implement this function
   */
  CmdNode *p = NULL;
  CmdNode *loop = NULL;
  p = head;
  int direction = 0; // 0 is right, 1 is down, 2 is left, 3 is up
  int draw = 0;      // 0 is pendown, 1 is penup
  int colour = 0;    // default colour
  int xcoor = 0;
  int ycoor = 0;

  while (p != NULL)
  {
    char cmd[10];
    int looptimes;
    strcpy(cmd, p->cmd);

    if (strcmp(cmd, "loop") == 0)
    {
      for (looptimes = 0; looptimes < p->val; looptimes++)
      {
        loop = p->loop_head;
        while (loop != NULL)
        {
          actualrun(im, &direction, &draw, &colour, &xcoor, &ycoor, loop->cmd, loop);
          loop = loop->next;
        }
      }
    }
    else 
    {
      actualrun(im, &direction, &draw, &colour, &xcoor, &ycoor, cmd, p);
    }
    p = p->next;

  }
  *endX = xcoor;
  *endY = ycoor;
}

/*---------------------------------------------------------------------------*/
// All done!