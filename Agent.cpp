#include "headers.h"

struct Prices {
    int tabaco;
    int matches;
    int paper;
};

struct Product {
    long mtype;
    int price;
};

void determining_the_stock_exchange_price(Prices *p);

int main(int argc, char *argv[]) {
    // Initializing the shared memory
    srand(time(NULL));

    // PRICES
    key_t key_prices = ftok("prices", 65);
    int shmid_prices = shmget(key_prices, sizeof(Prices), 0777);

    // If the message queue exists
    if (shmid_prices != -1) {
        if (shmctl(shmid_prices, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
        // Create the shared memory
        shmid_prices = shmget(key_prices, sizeof(Prices), 0777 | IPC_CREAT | IPC_EXCL);
        if (shmid_prices == -1) {
            perror("shmget");
            exit(1);
        }
    }

    // Attach the shared memory
    Prices *shm_prices = (Prices *)shmat(shmid_prices, NULL, 0);
    if (shm_prices == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    // MESSAGE QUEUES
    key_t key_tabaco = ftok("tabaco", 65);
    int msgid_tabaco = msgget(key_tabaco, 0777 | IPC_CREAT | IPC_EXCL);

    // If the message queue exists
    if (msgid_tabaco != -1) {
        // Remove the existing queue
        if (msgctl(msgid_tabaco, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(1);
        }
        // Create a new message queue
        msgid_tabaco = msgget(key_tabaco, 0777 | IPC_CREAT | IPC_EXCL);
        if (msgid_tabaco == -1) {
            perror("msgget");
            exit(1);
        }
    }

    // Repeat the same process for "paper" and "matches"
    key_t key_paper = ftok("paper", 65);
    int msgid_paper = msgget(key_paper, 0777 | IPC_CREAT | IPC_EXCL);

    if (msgid_paper != -1) {
        if (msgctl(msgid_paper, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(1);
        }
        msgid_paper = msgget(key_paper, 0777 | IPC_CREAT | IPC_EXCL);
        if (msgid_paper == -1) {
            perror("msgget");
            exit(1);
        }
    }

    key_t key_matches = ftok("matches", 65);
    int msgid_matches = msgget(key_matches, 0777 | IPC_CREAT | IPC_EXCL);

    if (msgid_matches != -1) {
        if (msgctl(msgid_matches, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(1);
        }
        msgid_matches = msgget(key_matches, 0777 | IPC_CREAT | IPC_EXCL);
        if (msgid_matches == -1) {
            perror("msgget");
            exit(1);
        }
    }

    while (true) {
        determining_the_stock_exchange_price(shm_prices);
        printf("Tabaco: %d\nPaper: %d\nMatches: %d\n\n", shm_prices->tabaco, shm_prices->paper, shm_prices->matches);
        sleep(1);
    }

    return 0;
}

void determining_the_stock_exchange_price(Prices *p) {
    p->tabaco = rand() % 10 + 1;
    p->paper = rand() % 10 + 1;
    p->matches = rand() % 10 + 1;
}