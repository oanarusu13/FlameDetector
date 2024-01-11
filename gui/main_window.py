import random

from PySide6.QtWidgets import QMainWindow, QHBoxLayout, QVBoxLayout, QWidget, QGroupBox, QLabel, QPushButton, QLineEdit, \
    QTextEdit
from PySide6.QtGui import QIcon, QPalette, QColor, QFont
from PySide6.QtCore import Qt, QTimer, QTime
import pyqtgraph as pg
import serial

ser = serial.Serial('COM6', 14400)
ox_counter = 1

class MainWindow(QMainWindow):
    promotie: str = "2023-2024"
    team: list[str] = [
        "Rusu Oana",
        "Grama Teona",
    ]

    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))

        primary_layout = QVBoxLayout()
        secondary_layout = QHBoxLayout()
        tertiary_layout = QVBoxLayout()

        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member, 1)
        team_box_layout.addWidget(second_member, 1)
        team_box.setLayout(team_box_layout)

        control_panel_box = QGroupBox("Control Panel")
        control_panel_box.setFont(bold_font)

        button1 = QPushButton("Reverse LEDs")
        button2 = QPushButton("Manual Servo Control")

        button3 = QPushButton("Send")
        button3.clicked.connect(self.send_input)
        button1.clicked.connect(self.reverse_leds)
        button2.clicked.connect(self.servo_manual)
        self.line_edit = QLineEdit()
        self.line_edit.setAlignment(Qt.AlignmentFlag.AlignBottom)
        line_edit_label = QLabel("Input:", parent=self.line_edit)
        control_panel_box_layout = QVBoxLayout()
        control_panel_box_layout.setSpacing(5)
        control_panel_box_layout.addWidget(button1, 1)
        control_panel_box_layout.addWidget(button2, 1)


        control_panel_box_layout.addStretch()
        control_panel_box_layout.addWidget(line_edit_label)
        control_panel_box_layout.addWidget(self.line_edit, 1)
        control_panel_box_layout.addWidget(button3, 1)

        control_panel_box.setLayout(control_panel_box_layout)

        tertiary_layout.addWidget(team_box, 1)
        tertiary_layout.addWidget(control_panel_box, 5)
        self.plot_widget = None
        #try:
        self.plot_widget = pg.PlotWidget()
        self.plot_widget.plotItem.setLabel('left', 'Flame Amplitudde')
        self.plot_widget.plotItem.setLabel('bottom', 'Samples')
        #except RuntimeError:
        #    print('ok')
        self.data = []
        hour = []
        flames = []

        self.plot_widget.plot(hour, flames)

        secondary_layout.addWidget(self.plot_widget, 3)
        secondary_layout.addLayout(tertiary_layout, 1)

        primary_layout.addLayout(secondary_layout, 4)
        self.text_edit = QTextEdit()
        self.text_edit.setReadOnly(True)

        debug_box = QGroupBox("Debug")
        debug_box_layout = QVBoxLayout()
        debug_box_layout.addWidget(self.text_edit, 1)
        debug_box.setLayout(debug_box_layout)

        primary_layout.addWidget(debug_box, 1)

        widget = QWidget()
        widget.setLayout(primary_layout)

        self.setCentralWidget(widget)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_graph)
        self.timer.start(50)

    def update_graph(self):
        global ox_counter
        new_second = ox_counter
        ox_counter += 1
        new_flame = 0
        while True:
            if ser.in_waiting > 0:
                new_flame = float(ser.readline())
                print("Received data:", new_flame)
            else:
                break

        self.data.append((new_second, new_flame))
        x_data, y_data = zip(*self.data)

        low_threshold = 1
        high_threshold = 2

        if new_flame < low_threshold:
            background_color = 'g'
        elif low_threshold <= new_flame < high_threshold:
            background_color = 'y'
        else:
            background_color = 'r'

        self.plot_widget.setBackground(background_color)

        self.plot_widget.clear()
        self.plot_widget.plot(x_data, y_data, pen='k')
        self.plot_widget.setXRange(max(0, new_second - 65), new_second)

    def reverse_leds(self):
        self.text_edit.insertPlainText(f"INPUT: r\n")
        ser.write('r'.encode())

    def servo_manual(self):
        self.text_edit.insertPlainText(f"INPUT: m\n")
        ser.write('m'.encode())

    def send_input(self):
        input = self.line_edit.text()
        self.line_edit.clear()
        self.text_edit.insertPlainText(f"INPUT: {input}\n")
        ser.write(input.encode())

    def read_serial_data(self):
        if self.ser.in_waiting > 0:
            return float(self.ser.readline().decode().strip())
        else:
            return 0.0
