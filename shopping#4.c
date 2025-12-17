#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTS 5

// 상품 정보를 담는 구조체 정의
typedef struct {
    int id;             // 상품 ID
    char name[50];      // 상품명
    int price;          // 상품 가격
    int input_qty;      // 입고량
    int sold_qty;       // 판매량
    int total_sales;    // 총 판매 금액
} Product;

// 전역 변수
Product products[MAX_PRODUCTS];
int product_count = 0; // 현재 등록된 상품 개수

// 함수 원형 선언
void menu_input();
void menu_sales();
void menu_individual_status();
void menu_overall_status();
int find_product_index(int id);

int main() {
    int choice;

    while (1) {
        // 메뉴 출력 (5번 종료로 변경)
        printf("\n=== [쇼핑몰 재고 관리 시스템] ===\n");
        printf("1. 입고\n");
        printf("2. 판매\n");
        printf("3. 개별현황\n");
        printf("4. 전체현황\n");
        printf("5. 종료\n");
        printf("메뉴를 선택하세요: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: menu_input(); break;
            case 2: menu_sales(); break;
            case 3: menu_individual_status(); break;
            case 4: menu_overall_status(); break;
            case 5: 
                printf("프로그램을 종료합니다.\n");
                return 0; // 프로그램 종료
            default: 
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
        }
    }
    return 0;
}

// 상품 ID로 배열 인덱스를 찾는 헬퍼 함수
int find_product_index(int id) {
    for (int i = 0; i < product_count; i++) {
        if (products[i].id == id) {
            return i;
        }
    }
    return -1;
}

// 1. 입고 메뉴 
void menu_input() {
    int id, input_q, price;
    char name[50];
    
    printf("\n[입고 메뉴]\n");
    printf("상품 ID 입력: ");
    scanf("%d", &id);

    int index = find_product_index(id);

    if (index != -1) {
        // 기존 상품 업데이트 (이미 존재하는 ID일 경우)
        printf(">> 기존 상품(ID: %d, 이름: %s)이 존재합니다. 추가 입고를 진행합니다.\n", id, products[index].name);
        printf("추가 입고량: ");
        scanf("%d", &input_q);
        products[index].input_qty += input_q;
        printf("입고 완료! (현재 재고: %d)\n", products[index].input_qty - products[index].sold_qty);
    } else {
        // 신규 상품 등록
        if (product_count >= MAX_PRODUCTS) {
            // 상품 개수가 5개를 초과할 경우 메시지 출력
            printf("오류: 더 이상 상품을 등록할 수 없습니다. (최대 %d개)\n", MAX_PRODUCTS);
            return;
        }

        printf("신규 상품명: ");
        scanf("%s", name);
        printf("판매 가격: ");
        scanf("%d", &price);
        printf("입고량: ");
        scanf("%d", &input_q);

        products[product_count].id = id;
        strcpy(products[product_count].name, name);
        products[product_count].price = price;
        products[product_count].input_qty = input_q;
        products[product_count].sold_qty = 0;
        products[product_count].total_sales = 0;
        
        product_count++;
        printf("신규 상품 등록 완료!\n");
    }
}

// 2. 판매 메뉴 
void menu_sales() {
    int id, sell_q;

    printf("\n[판매 메뉴]\n");
    printf("상품 ID 입력: ");
    scanf("%d", &id);

    int index = find_product_index(id);
    if (index == -1) {
        printf("오류: 존재하지 않는 상품 ID입니다.\n");
        return;
    }

    printf("판매할 수량 입력: ");
    scanf("%d", &sell_q);

    int current_stock = products[index].input_qty - products[index].sold_qty;
    
    // 재고 부족 시 판매 불가 처리 (요구사항에는 명시되지 않았으나 논리적 필수)
    if (sell_q > current_stock) {
        printf("오류: 재고가 부족합니다. (현재 재고: %d)\n", current_stock);
    } else {
        products[index].sold_qty += sell_q;
        products[index].total_sales += (sell_q * products[index].price);
        printf("판매 완료! (누적 판매량: %d, 총 매출: %d)\n", products[index].sold_qty, products[index].total_sales);
    }
}

// 3. 개별 현황 메뉴 
void menu_individual_status() {
    int id;
    printf("\n[개별 상품 정보 조회]\n");
    printf("조회할 상품 ID: ");
    scanf("%d", &id);

    int index = find_product_index(id);
    if (index == -1) {
        printf("존재하지 않는 상품입니다.\n");
        return;
    }

    Product p = products[index];
    int stock = p.input_qty - p.sold_qty;
    
    printf("--- 상품 정보 ---\n");
    printf("ID: %d\n", p.id);
    printf("상품명: %s\n", p.name);
    printf("가격: %d원\n", p.price);
    printf("입고량: %d, 판매량: %d\n", p.input_qty, p.sold_qty);
    printf("현재재고: %d\n", stock);
    printf("총판매금액: %d원\n", p.total_sales);
}

// 4. 전체 현황 메뉴 
void menu_overall_status() {
    int total_stock = 0;
    int total_sold = 0;
    int total_input = 0;
    
    // 최대/최소 판매 상품 찾기용 변수
    int max_idx = -1, min_idx = -1;
    int max_sold = -1, min_sold = -1; // 초기값 설정

    printf("\n[전체 상품 현황]\n");
    printf("ID\t상품명\t재고\t판매량\t상태\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < product_count; i++) {
        int current_stock = products[i].input_qty - products[i].sold_qty;
        total_stock += current_stock;
        total_sold += products[i].sold_qty;
        total_input += products[i].input_qty;

        // 최대 판매량 갱신 로직
        if (max_idx == -1 || products[i].sold_qty > max_sold) {
            max_sold = products[i].sold_qty;
            max_idx = i;
        }
        // 최소 판매량 갱신 로직
        if (min_idx == -1 || products[i].sold_qty < min_sold) {
            min_sold = products[i].sold_qty;
            min_idx = i;
        }

        printf("%d\t%s\t%d\t%d\t", products[i].id, products[i].name, current_stock, products[i].sold_qty);
        
        // 재고 부족 경고 (재고 2개 이하일 때)
        if (current_stock <= 2) {
            printf("재고부족(%d)", current_stock);
        }
        printf("\n");
    }
    printf("----------------------------------------\n");
    
    // 판매율 계산 (0으로 나누기 방지)
    double sales_rate = 0.0;
    if (total_input > 0) {
        sales_rate = ((double)total_sold / total_input) * 100.0;
    }

    printf("총 재고 수량: %d\n", total_stock);
    printf("총 판매량: %d (판매율: %.2f%%)\n", total_sold, sales_rate); 

    if (max_idx != -1)
        printf("가장 많이 판매된 상품: ID %d (%s), %d개\n", products[max_idx].id, products[max_idx].name, products[max_idx].sold_qty);
    if (min_idx != -1)
        printf("가장 적게 판매된 상품: ID %d (%s), %d개\n", products[min_idx].id, products[min_idx].name, products[min_idx].sold_qty);
}
