Из лекции 51 на ютубе
### Домашнее задание по теме "Канальный уровень и Wireshark"

#### Цель:
Изучить структуру заголовков канального уровня, научиться использовать Wireshark для анализа сетевого трафика и заполнять заголовки на канальном, сетевом и транспортном уровнях.

#### Задания:

1. **Изучение заголовков канального уровня**:
   - Прочитайте теорию о заголовках канального уровня, включая MAC-адреса, их структуру и назначение.
   - Изучите, как MAC-адреса используются для адресации на канальном уровне и как они отличаются от IP-адресов.

2. **Практическое использование Wireshark**:
   - Установите и запустите Wireshark на вашем компьютере.
   - Захватите сетевой трафик между двумя устройствами в вашей сети.
   - Используйте фильтры для отсеивания ненужного трафика и анализа интересующих пакетов.
   - Найдите и проанализируйте процесс установления TCP-соединения (тройное рукопожатие).

3. **Заполнение заголовков на канальном уровне**:
   - Создайте программу, которая заполняет заголовок канального уровня (например, с использованием структуры `sockaddr_ll`).
   - Заполните поля MAC-адреса источника и назначения, а также поле `sll_protocol`.
   - Отправьте и получите данные с использованием созданного сокета, анализируя заголовки на всех уровнях (канальный, сетевой, транспортный).

4. **Анализ сетевого трафика**:
   - Используйте Wireshark для анализа захваченного трафика.
   - Визуализируйте обмен данными между клиентом и сервером с помощью графиков и диаграмм.
   - Изучите статистику сетевого трафика, включая пропускную способность и задержки.

5. **Дополнительные задания**:
   - Изучите, как вычисляется контрольная сумма для IP-заголовка и реализуйте этот алгоритм в коде.
   - Настройте виртуальную сеть и протестируйте обмен данными между виртуальной машиной и хост-машиной.