/* Print which syntax bits are set.  */

#include <sys/types.h>
#include <stdio.h>
#include "regex.h"

/* It's coincidental that these two are currently the same.  */
#define LONGEST_BIT_NAME "RE_UNMATCHED_RIGHT_PAREN_ORD"
#define LAST_BIT RE_UNMATCHED_RIGHT_PAREN_ORD

/* Sum of above, when printed.  Assigned in main.  */
static unsigned longest;


static void
test_bit (syntax, bit, name)
    reg_syntax_t syntax;
    unsigned bit;
    char *name;
{
  char padding[100], test_str[100];
  int padding_count;
  
  sprintf (test_str, "%s (%d=0x%x)", name, bit, bit);
  padding_count = longest - strlen (test_str);

  padding[padding_count] = 0;
  while (padding_count--)
    {
      padding[padding_count] = ' ';
    }
  
  printf ("%s%s (%d=0x%x): %c\n",
          name, padding, bit, bit, syntax & bit ? 'y' : 'n');
}


/* Macro to abbreviate the constant arguments.  */
#define TEST_BIT(bit) test_bit (syntax, bit, #bit)

int
main (argc, argv)
    int argc;
    char *argv[];
{
  reg_syntax_t syntax;
  char syntax_str[1000], test_str[100];
  
  switch (argc)
    {
    case 1:
      printf ("Syntax? ");
      scanf ("%s", syntax_str);
      break;
    
    case 2:
      strcpy (syntax_str, argv[1]);
      break;

    default:
      fprintf (stderr, "Usage: syntax [syntax].\n");
      exit (1);
    }

  sscanf (syntax_str, "%i", &syntax);
  
  /* Figure out the longest name, so we can align the output nicely.  */
  sprintf (test_str, "%s (%d=0x%x)", LONGEST_BIT_NAME, LAST_BIT, LAST_BIT);
  longest = strlen (test_str);

  /* [[[replace with bit tests]]] */
  TEST_BIT (RE_BACKSLASH_ESCAPE_IN_LISTS);
  TEST_BIT (RE_BK_PLUS_QM);
  TEST_BIT (RE_CHAR_CLASSES);
  TEST_BIT (RE_CONTEXT_INDEP_ANCHORS);
  TEST_BIT (RE_CONTEXT_INDEP_OPS);
  TEST_BIT (RE_CONTEXT_INVALID_OPS);
  TEST_BIT (RE_DOT_NEWLINE);
  TEST_BIT (RE_DOT_NOT_NULL);
  TEST_BIT (RE_HAT_LISTS_NOT_NEWLINE);
  TEST_BIT (RE_INTERVALS);
  TEST_BIT (RE_LIMITED_OPS);
  TEST_BIT (RE_NEWLINE_ALT);
  TEST_BIT (RE_NO_BK_BRACES);
  TEST_BIT (RE_NO_BK_PARENS);
  TEST_BIT (RE_NO_BK_REFS);
  TEST_BIT (RE_NO_BK_VBAR);
  TEST_BIT (RE_NO_EMPTY_RANGES);
  TEST_BIT (RE_UNMATCHED_RIGHT_PAREN_ORD);
  
  return 0;
}
