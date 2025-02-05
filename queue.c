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
    if (!head)
        return 0;
    int count = 0;
    struct list_head *pos;
    list_for_each (pos, head) {
        count++;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return false;  // 若佇列為 NULL 或為空，返回 false
    }
    struct list_head *slow = head->next, *fast = head->next;
    // 使用快慢指標尋找中間節點
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // 轉換 slow 指標為 element_t
    element_t *mid_element = list_entry(slow, element_t, list);
    // 從鏈結串列中移除該節點
    list_del(slow);
    // 釋放記憶體
    q_release_element(mid_element);
    return true;  // 成功刪除中間節點
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return false;  // 若佇列為 NULL 或為空，返回 false
    }

    struct list_head *cur = head->next, *tmp;
    bool deleted = false;  // 標記是否刪除了節點

    while (cur != head) {
        element_t *elem = list_entry(cur, element_t, list);
        tmp = cur->next;

        bool has_dup = false;
        while (tmp != head) {
            element_t *next_elem = list_entry(tmp, element_t, list);
            if (strcmp(elem->value, next_elem->value) == 0) {
                has_dup = true;
                struct list_head *dup = tmp;
                tmp = tmp->next;
                list_del(dup);
                q_release_element(next_elem);  // 釋放重複節點的記憶體
            } else {
                break;
            }
        }

        if (has_dup) {
            // 也刪除當前節點
            struct list_head *dup = cur;
            cur = tmp;
            list_del(dup);
            q_release_element(elem);
            deleted = true;
        } else {
            cur = tmp;
        }
    }

    return deleted;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;  // 如果佇列為 NULL、空，或只有一個元素，則無需交換
    }
    struct list_head *cur = head->next;
    while (cur != head && cur->next != head) {
        struct list_head *next = cur->next;  // 獲取下一個節點
        list_move(cur, next);  // 交換當前節點與下一個節點
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;  // 若佇列為 NULL、空，或只有一個元素，則無需反轉
    }
    struct list_head *cur, *tmp;
    list_for_each_safe (cur, tmp, head) {
        list_move(cur, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || k < 2) {
        return;  // 若佇列為 NULL、空，或 k < 2 則無需反轉
    }
    struct list_head *cur = head->next, *group_tail = head, *tmp;
    int count = 0;
    while (cur != head) {
        count++;
        if (count % k == 0) {
            struct list_head *group_head = group_tail->next;
            struct list_head *next_group = cur->next;
            // 反轉當前 k 個節點
            struct list_head *node = group_head;
            struct list_head *end = next_group;
            while (node != next_group) {
                tmp = node->next;
                list_move(node, end);
                node = tmp;
            }
            group_tail = group_head;  // 更新 group_tail
            cur = next_group;         // 更新當前指標
        } else {
            cur = cur->next;
        }
    }
}

void q_merge_two(struct list_head *first, struct list_head *second)
{
    struct list_head sorted;
    INIT_LIST_HEAD(&sorted);

    while (!list_empty(first) && !list_empty(second)) {
        element_t *e1 = list_entry(first->next, element_t, list);
        element_t *e2 = list_entry(second->next, element_t, list);

        if (strcmp(e1->value, e2->value) <= 0)
            list_move_tail(first->next, &sorted);
        else
            list_move_tail(second->next, &sorted);
    }

    /* Append remaining elements */
    list_splice_tail(first, &sorted);
    list_splice_tail(second, &sorted);

    /* Copy sorted list back */
    list_splice(&sorted, first);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    /* Find the middle point using slow/fast pointer */
    struct list_head *mid, *slow, *fast;
    slow = fast = head->next;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    mid = slow;
    /* Divide into two parts */
    LIST_HEAD(second);
    list_cut_position(&second, mid, head->prev);

    /* Sort each part */
    q_sort(head, false);
    q_sort(&second, false);

    /* Merge two sorted lists */
    q_merge_two(head, &second);

    /* If descending order is required, reverse the list */
    if (descend)
        q_reverse(head);
}

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
