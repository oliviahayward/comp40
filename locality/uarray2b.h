#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED
#define T UArray2b_T
typedef struct T *T;

/* Function: UArray2b_new
 * Input: width, height, size of elements, blocksize
 * Returns: struct UArray2b_T array
 * Does: Creates a new instance of UArray2b_T with the specified information
 * Error cases: If the blocksize, width, height, or size are less than 1, exit
 *              the program. If malloc() returns NULL, exit the program.
 */
extern T UArray2b_new (int width, int height, int size, int blocksize);

/* Function: UArray2b_new_64k_block
 * Input: width, height, size of elements
 * Returns: struct UArray2b_T array
 * Does: Creates a new instance of UArray2b_T array with the given
 *       specifications and an optimized blocksize
 * Error cases: If the width, height, or size is less than 1, exit the program.
 *              If malloc() returns NULL, exit the program.
 */
extern T UArray2b_new_64K_block(int width, int height, int size);

/* Function: UArray2b_free
 * Input: a pointer to a UArray2b_T array struct
 * Returns: nothing
 * Does: Frees the memory allocated to that struct and the pointer
 * Error cases: If the struct is NULL or if the pointer to the struct is NULL,
 *              exit the program.
 */
extern void UArray2b_free (T *array2b);

/* Function: UArray2b_width
 * Input: UArray2b_T array struct
 * Returns: the width of the 2D array
 * Does: Returns the width of the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
extern int UArray2b_width (T array2b);

/* Function: UArray2b_height
 * Input: UArray2b_T array struct
 * Returns: the height of the 2D array
 * Does: Returns the height of the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
extern int UArray2b_height (T array2b);

/* Function: UArray2b_size
 * Input: UArray2b_T array struct
 * Returns: the size of the elements in the 2D array
 * Does: Returns the size of the elements in the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
extern int UArray2b_size (T array2b);

/* Function: UArray2b_blocksize
 * Input: UArray2b_T array struct
 * Returns: the blocksize of the 2D array
 * Does: Returns the blocksize of the 2D array
 * Error cases: If the struct is NULL, exit the program.
 */
extern int UArray2b_blocksize(T array2b);

/* Function: UArray2b_at
 * Input: UArray2b_T array struct, column number, and row number
 * Returns: A void pointer to the element at that coordinate in the array
 * Does: It calculates the number of blocks per row and gets the block
 *       coordinates. It uses that to get the index of the block that the
 *       coordinate is located in. It uses that to calculate the
 *       beginning index of that block and then uses modulo arithmetic to
 *       determine the index of the element.
 * Error cases: If the UArray2b_T struct is NULL, exit the program. If the
 *              column or row number is less than 0 or greater than the
 *              dimensions of the image, exit the program. If the calculated
 *              index is larger than the length of the array or is less than
 *              0, exit the program.
 */
extern void *UArray2b_at(T array2b, int column, int row);

/* Function: UArray2b_map
 * Input: UArray2b_T array struct, apply function, void pointer to a closure
 *        argument
 * Returns: Nothing
 * Does: Iterates throught the array and applies the apply function to every
 *       element in the array
 * Error cases: If the struct, the struct's array, or the apply function is
 *              NULL, exit the program.
 */
extern void UArray2b_map(T array2b,
void apply(int col, int row, T array2b,
void *elem, void *cl),
void *cl);

#undef T
#endif