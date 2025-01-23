#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_queue = malloc(sizeof(struct list_head));
    if (!new_queue) {
        return NULL;
    }
    INIT_LIST_HEAD(new_queue);
    return new_queue;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;  // 如果 head 為 NULL，直接返回
    }

    struct list_head *current, *tmp;

    // 安全遍歷佇列，並釋放節點記憶體
    list_for_each_safe (current, tmp, head) {
        element_t *entry = list_entry(current, element_t, list);
        q_release_element(entry);  // 釋放元素的記憶體
    }

    // 釋放佇列頭節點
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;  // 如果佇列頭或字串為 NULL，返回 false
    }
    // 分配記憶體給新的元素
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element) {
        return false;  // 如果記憶體分配失敗，返回 false
    }
    // 為字串分配記憶體並複製內容
    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);  // 如果字串分配失敗，釋放節點記憶體
        return false;
    }
    // 將新節點加入到佇列的開頭
    list_add(&new_element->list, head);
    return true;  // 插入成功
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;  // 若佇列或字串為 NULL，返回失敗
    }
    // 分配記憶體給新節點
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element) {
        return false;  // 若記憶體分配失敗，返回失敗
    }
    // 分配記憶體給字串，並複製內容
    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);  // 若字串記憶體分配失敗，釋放節點
        return false;
    }
    // 將新節點插入到佇列的尾端
    list_add_tail(&new_element->list, head);
    return true;  // 插入成功
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;  // 若佇列為 NULL 或為空，返回 NULL
    }
    // 獲取佇列的第一個節點
    struct list_head *first = head->next;
    element_t *element = list_entry(first, element_t, list);
    // 如果提供了緩衝區，將字串複製到緩衝區
    if (sp) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';  // 確保以 NULL 結尾
    }
    // 從佇列中移除該節點
    list_del(first);
    return element;  // 返回移除的節點指標
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;  // 若佇列為 NULL 或為空，返回 NULL
    }

    // 獲取佇列的最後一個節點
    struct list_head *last = head->prev;
    element_t *element = list_entry(last, element_t, list);
    // 如果提供了緩衝區，將字串複製到緩衝區
    if (sp) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';  // 確保以 NULL 結尾
    }
    // 從佇列中移除該節點
    list_del(last);
    return element;  // 返回移除的節點指標
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    return -1;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
