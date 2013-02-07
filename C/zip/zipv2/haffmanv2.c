#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;


//*******************************
//***************生成树阶段******
//*******************************
enum {UNINIT, UNUSED, USED};

struct tree_t {
	UINT16 c;		//权值
	UINT16 l;		//左子树
	UINT16 r;		//右子树
	UINT8 f;			//标志位
	UINT8 v;			//值
};

#define MAX_NODE	512
static struct tree_t t[MAX_NODE];	//存256个叶子节点，及它们的父节点

static int __get_min1(const struct tree_t *t){
	int i, min = -1;

	//找到第一个未使用的最为最小值
	for (i = 0; i < MAX_NODE; i++) 
		if (t[i].f == UNUSED) 
			break;
	if (i == MAX_NODE)
		goto _exit;
	min = i;
	//和后面的一次比较，找到一个最小的
	for (i++; i < MAX_NODE; i++) {
		if (t[i].f == UNUSED)
			if (t[i].c < t[min].c)
				min = i;
	}

_exit:
	//fprintf(stderr, "get min1 %d\n", min);
	return min;
}
//返回最小的2个,其下标未a和b，并将其标为使用
static int get_min2(struct tree_t *t, UINT16 *a, UINT16 *b) {
	int ret = 0, min;

	if ((min = __get_min1(t)) != -1) {
		t[min].f = USED;
		*a = min;
		ret++;
	}
	if ((min = __get_min1(t)) != -1) {
		t[min].f = USED;
		*b = min;
		ret++;
	}

	//fprintf(stderr, "get_min2: %d, %d\n", *a, *b);
	return ret;
}

static void dump_t(const struct tree_t *t) {
	int i;
	for (i = 0; i < MAX_NODE; i++) {
		//if (t[i].f != USED)
		if (t[i].c == 0)
			continue;
		fprintf(stderr, "t[%d].c = %d, .l = %d, .r = %d, .v = %d\n", \
				i, t[i].c, t[i].l, t[i].r, t[i].v);
	}
}

static int scan_data_to_arr(const UINT8 *data, int len, struct tree_t *t) {
	int i, n = 256;
	UINT16 a, b;
	
	if (len == 0)
		return -1;

	//扫描数据源，初始所有的叶子节点
	memset(t, 0, sizeof(struct tree_t) * MAX_NODE);
	for (i = 0; i < 256; i++) { 
		t[i].v = i;
	}
	for (i = 0; i < len; i++)
		t[data[i]].c++;
	for (i = 0; i < 256; i++)
		if (t[i].c > 0)
			t[i].f = UNUSED;

	//dump_t(t);
	//从叶子节点中找到2个权值最小的，
	//标为已用，再申请一个未用的节点，其左值未a，右值为b，权值未a+b， flag未未用。
	//直至只有一个未所用的，其即为树根
	while (get_min2(t, &a, &b) == 2) {
		t[n].c = t[a].c + t[b].c;
		t[n].l = a;
		t[n].r = b;
		t[n].f = UNUSED;
		t[n].v = 0;
		n++;
	}

	fprintf(stderr, "the tree root is %d\n", a);
	dump_t(t);

	return a;
}

//*******************************
//***************保存树阶段******
//*******************************

enum {L, R};
struct node_t {
	UINT8 d[2];	//如果孩子不是叶子节点，存储它们的下标
	UINT8 v[2];	//如果孩子是叶子节点，则存储值
};

//采用先序遍历将树存进数组中
//参数r为数根

static const struct tree_t *_t;
static struct node_t *_data;
static UINT8 _n;

static void __save_tree(int r, int p);

static int is_leaf_node(UINT16 r) {
	return (_t[r].l == _t[r].r && _t[r].l == 0);
}

static void __save_tree_l(int r, int p) {
	//fprintf(stderr, "%s(%d), _n = %d, p = %d\n", __FUNCTION__, r, _n, p);
	if (!is_leaf_node(_t[r].l))  { //不是叶子节点
		_data[p].d[L] = ++_n;		//保存下一个节点的下标
		_data[p].v[L] = 0;	

		p = _n;		//下一个节点保存在数组的_n下标里
		r = _t[r].l;
		__save_tree(r, p);			//遍历下一个节点
	} else {
		_data[p].d[L] = 0;
		_data[p].v[L] = _t[_t[r].l].v;	//保存叶子节点值
	}
}

static void __save_tree_r(int r, int p) {
	//fprintf(stderr, "%s(%d), _n = %d, p = %d\n", __FUNCTION__, r, _n, p);
	if (!is_leaf_node(_t[r].r))  { 	//不是叶子节点
		_data[p].d[R] = ++_n;		//保存下一个节点的下标
		_data[p].v[R] = 0;	

		p = _n;		//下一个节点保存在数组的_n下标里
		r = _t[r].r;			
		__save_tree(r, p);			//遍历下一个节点
	} else {
		_data[p].d[R] = 0;
		_data[p].v[R] = _t[_t[r].r].v;	//保存叶子节点值
	}
}

static void __save_tree(int r, int p) {
	__save_tree_l(r, p);
	__save_tree_r(r, p);
}

static void dump_node(const UINT8 *out) {
	struct node_t *p = (void *)out;
	int i;

	fprintf(stderr, "the code is \n");
	for (i = 0; i <= _n; i++) 
		fprintf(stderr, "[%d]\t", i);
	fprintf(stderr, "\n");
	for (i = 0; i <= _n; i++) 
		fprintf(stderr, "0->[%d]\t", p[i].d[L]);
	fprintf(stderr, "\n");
	for (i = 0; i <= _n; i++) 
		fprintf(stderr, "1->[%d]\t", p[i].d[R]);
	fprintf(stderr, "\n");
	for (i = 0; i <= _n; i++) 
		fprintf(stderr, "%d\t", p[i].v[L]);
	fprintf(stderr, "\n");
	for (i = 0; i <= _n; i++) 
		fprintf(stderr, "%d\t", p[i].v[R]);
	fprintf(stderr, "\n");
}

static int save_tree(const struct tree_t *t, int r, UINT8 *out) {
	int p = 0;
	_n = 0;
	_data = (void *)out;
	_t = t;

	__save_tree(r, p);

	dump_node(out);

	return _n+1;
}

//*******************************
//***************编码阶段******
//*******************************

static UINT8 bit_buf;
static UINT8 bit_point;
static int ring_counter;
static void reset_bit_ring(void) {
	bit_buf = 0;
	bit_point = 0;
	ring_counter = 0;
}
static int flush_bit_ring(UINT8 *buf) {
	bit_buf <<= (7 - bit_point);
	buf[ring_counter] = bit_buf;
	ring_counter++;
	return ring_counter;
}
static void bit_to_buf(UINT8 *buf, const UINT8 *bit, int bit_len) {
	int i;
	for (i = 0; i < bit_len; i++) {
		bit_buf |= bit[i] ? 1 : 0;
		bit_buf <<= 1;
		bit_point++;
		if (bit_point == 8) {
			bit_point = 0;
			bit_buf = 0;
			bit_point = 0;
			buf[ring_counter++] = bit_buf;
		}
	}
}

static int dep_len;
static int find_it = 0;
static void byte_to_bit(const struct tree_t *root, UINT8 byte, \
								UINT8 *arr, UINT8 dep) 
{
	

	if (!find_it && root->l != 0) {
		byte_to_bit(&t[root->l], byte, arr, dep+1);
		if (find_it)
			arr[dep] = '0';
	} else {
		if (root->v == byte) {
			find_it = 1;
			dep_len = dep;
			return;
		}
	}


	if (!find_it && root->r != 0) {
		byte_to_bit(&t[root->r], byte, arr, dep+1);
		if (find_it)
			arr[dep] = '1';
	} else {
		if (root->v == byte) {
			dep_len = dep;
			find_it = 1;
			return;
		}
	}


}

static int encode(const struct tree_t *root, const UINT8 *data, \
						int len, UINT8* buf) 
{
	int i;
	UINT8 bit[256];
	
	reset_bit_ring();
	for (i = 0; i < len; i++) {
		memset(bit, 0, sizeof(bit));
		find_it = 0;
		byte_to_bit(root, data[i], bit, 0);	//获取单个字节的编码
		fprintf(stderr, "%c:%s\n", data[i], bit);
		bit_to_buf(buf, bit, dep_len);		//发送b个比特位到buf中
	}
	
	return flush_bit_ring(buf);
}


static UINT8 data[1024000];	//1M
static UINT8 code[1024000];
int main(int argc, char **argv) {
	FILE *fp;
	int ret = -1, len, len1,root;

	//if (argc != 2) {
	//	fprintf(stderr, "Usage %s file_name\n", argv[0]);
	//	goto _exit;
	//}

	//if ((fp = fopen(argv[1], "r")) == NULL) {
	if ((fp = fopen("test.txt", "r")) == NULL) {
		fprintf(stderr, "fopen %s failed\n", argv[1]);
		goto _exit;
	}

	len1 = len = fread(data, 1, sizeof(data), fp);

	root = scan_data_to_arr(data, len, t);

	if (root != -1)
		len = save_tree(t, root, code);
	encode(&t[root], data, len1, code+len);
	
	ret = 0;
	fclose(fp);
_exit:
	return ret;
}
