
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

#define MAX 10

struct Order {
    int id;
    char item[20];
    int price;
};

Order q[MAX];
int front = 0, rear = 0, count = 0;
int orderId = 1;

char statusMessage[100] = "Welcome to Campus Bites!";
int statusColor = BLUE;

// Display text with the correct background colour
void text(int x, int y, const char *s,
          int color, int bg, int size) {
    setcolor(color);
    setbkcolor(bg);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, size);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(x, y, (char*)s);
}

// Draw a coloured panel
void panel(int x1, int y1, int x2, int y2, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x1, y1, x2, y2);
    setcolor(DARKGRAY);
    rectangle(x1, y1, x2, y2);
}

// Draw the food illustrations
void foodCard(int x, const char *name,
              const char *price, char type, int cardColor) {
    panel(x, 125, x + 215, 250, cardColor);

    // Burger
    if (type == 'B') {
        setcolor(BROWN);
        setfillstyle(SOLID_FILL, BROWN);
        fillellipse(x + 42, 163, 28, 12);

        setfillstyle(SOLID_FILL, YELLOW);
        bar(x + 15, 164, x + 69, 169);

        setfillstyle(SOLID_FILL, GREEN);
        bar(x + 16, 170, x + 68, 175);

        setfillstyle(SOLID_FILL, RED);
        bar(x + 17, 176, x + 67, 180);

        setfillstyle(SOLID_FILL, BROWN);
        fillellipse(x + 42, 181, 27, 7);
    }

    // Pizza slice
    else if (type == 'P') {
        int points[] = {
            x + 12, 140,
            x + 72, 140,
            x + 43, 198,
            x + 12, 140
        };

        setcolor(BROWN);
        setfillstyle(SOLID_FILL, YELLOW);
        fillpoly(4, points);

        setfillstyle(SOLID_FILL, RED);
        fillellipse(x + 34, 151, 4, 4);
        fillellipse(x + 52, 153, 4, 4);
        fillellipse(x + 43, 169, 4, 4);
    }

    // Sandwich
    else if (type == 'S') {
        int points[] = {
            x + 12, 190,
            x + 42, 140,
            x + 72, 190,
            x + 12, 190
        };

        setcolor(BROWN);
        setfillstyle(SOLID_FILL, LIGHTRED);
        fillpoly(4, points);

        setcolor(GREEN);
        line(x + 25, 170, x + 59, 170);

        setcolor(YELLOW);
        line(x + 30, 179, x + 54, 179);
    }

    // Cold drink
    else {
        setcolor(BLUE);
        setfillstyle(SOLID_FILL, LIGHTCYAN);
        bar(x + 25, 148, x + 59, 194);
        rectangle(x + 25, 148, x + 59, 194);

        setcolor(RED);
        line(x + 45, 133, x + 39, 157);
    }

    // Text matches the card background
    text(x + 82, 155, name, BLACK, cardColor, 2);
    text(x + 82, 190, price, BLUE, cardColor, 2);
    text(x + 15, 220, "Fresh and tasty!", DARKGRAY,
         cardColor, 1);

    setbkcolor(WHITE);
}

// Draw a button
void button(int x, int y, int w,
            const char *label, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x, y, x + w, y + 43);

    setcolor(DARKGRAY);
    rectangle(x, y, x + w, y + 43);

    text(x + 10, y + 13, label, WHITE, color, 2);
    setbkcolor(WHITE);
}

// Add an order to the queue
void addOrder(const char *name, int price) {
    if (count >= MAX) {
        strcpy(statusMessage,
               "Queue is full! Serve an order first.");
        statusColor = RED;
        return;
    }

    q[rear].id = orderId++;
    strcpy(q[rear].item, name);
    q[rear].price = price;

    rear = (rear + 1) % MAX;
    count++;

    sprintf(statusMessage, "Order added successfully! Order #%d",
            orderId - 1);
    statusColor = GREEN;
}

// Serve the first order
void serveOrder() {
    if (count == 0) {
        strcpy(statusMessage, "No pending orders to serve!");
        statusColor = RED;
        return;
    }

    int servedId = q[front].id;
    char servedItem[20];
    strcpy(servedItem, q[front].item);

    front = (front + 1) % MAX;
    count--;

    sprintf(statusMessage, "Order #%d (%s) served successfully!",
            servedId, servedItem);
    statusColor = GREEN;
}

// Display pending orders and bill
void displayOrders() {
    panel(30, 370, 970, 610, WHITE);

    text(50, 385, "PENDING ORDERS (FIFO)",
         BLUE, WHITE, 2);

    char info[50];
    sprintf(info, "Pending: %d / %d", count, MAX);
    text(745, 390, info, BLUE, WHITE, 1);

    int total = 0;
    int y = 430;

    if (count == 0) {
        text(55, 450, "No pending orders. Place your first order!",
             DARKGRAY, WHITE, 2);
    }

    // Show up to five pending orders
    for (int i = 0; i < count && i < 5; i++) {
        int index = (front + i) % MAX;
        char line[100];

        sprintf(line, "Order #%d: %s   Rs. %d",
                q[index].id, q[index].item, q[index].price);

        panel(50, y - 4, 590, y + 24, LIGHTCYAN);
        text(60, y, line, BLACK, LIGHTCYAN, 1);

        y += 32;
    }

    // Calculate the bill for ALL pending orders
    for (int i = 0; i < count; i++) {
        int index = (front + i) % MAX;
        total += q[index].price;
    }

    panel(650, 440, 940, 540, LIGHTGRAY);
    text(675, 455, "PENDING TOTAL", DARKGRAY, LIGHTGRAY, 2);

    char bill[40];
    sprintf(bill, "Rs. %d", total);
    text(675, 495, bill, BLUE, LIGHTGRAY, 3);

    if (count > 5) {
        text(55, 590, "More orders are pending in the queue.",
             DARKGRAY, WHITE, 1);
    }
}

// Draw the complete screen
void drawScreen() {
    setbkcolor(WHITE);
    cleardevice();

    // Header
    setfillstyle(SOLID_FILL, BLUE);
    bar(0, 0, 1000, 90);

    text(300, 18, "CAMPUS BITES", WHITE, BLUE, 4);
    text(270, 63, "YOUR CAMPUS CAFE - FRESH FOOD, HAPPY MOOD",
         WHITE, BLUE, 1);

    // Menu
    text(35, 98, "TODAY'S MENU", DARKGRAY, WHITE, 2);

    foodCard(30, "Burger", "Rs. 60", 'B', LIGHTGREEN);
    foodCard(270, "Pizza", "Rs. 100", 'P', LIGHTMAGENTA);
    foodCard(510, "Sandwich", "Rs. 50", 'S', LIGHTCYAN);
    foodCard(750, "Cold Drink", "Rs. 30", 'D', LIGHTRED);

    // Buttons
    text(35, 260, "PLACE YOUR ORDER", DARKGRAY, WHITE, 2);

    button(30, 285, 145, "1 Burger", GREEN);
    button(190, 285, 145, "2 Pizza", MAGENTA);
    button(350, 285, 170, "3 Sandwich", BLUE);
    button(535, 285, 170, "4 Drink", RED);
    button(720, 285, 125, "5 Serve", DARKGRAY);
    button(860, 285, 110, "ESC Exit", BLUE);

    // Queue and total
    displayOrders();

    // Status message
    panel(30, 620, 970, 660, LIGHTCYAN);
    text(45, 635, statusMessage, statusColor, LIGHTCYAN, 1);

    text(35, 680,
         "CAMPUS BITES | Queue-based Order Management System",
         DARKGRAY, WHITE, 1);
}

int main() {
    initwindow(1000, 720, "Campus Bites - College Canteen");

    while (1) {
        drawScreen();

        char key = getch();

        if (key == 27) {
            break;
        }

        switch (key) {
            case '1':
                addOrder("Burger", 60);
                break;

            case '2':
                addOrder("Pizza", 100);
                break;

            case '3':
                addOrder("Sandwich", 50);
                break;

            case '4':
                addOrder("Cold Drink", 30);
                break;

            case '5':
                serveOrder();
                break;
        }
    }

    closegraph();
    return 0;
}
