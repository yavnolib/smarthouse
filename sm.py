def clean_display1():
    destroy_object1 = [log_button, log_label]
    for object_name in destroy_object1:
        object_name.destroy()

    display2()
def Button_on():
    global but
    but+=1
def display2():
    global but
    name = StringVar()
    surname = StringVar()
    passw=StringVar()
    name_label = Label(text="Введите имя:")
    surname_label = Label(text="Введите фамилию:")
    passw_label = Label(text='Введите пароль:')

    name_label.grid(row=0, column=8, sticky="w")
    surname_label.grid(row=1, column=8, sticky="w")
    passw_label.grid(row=2, column=8, sticky='w')

    name_entry = Entry(textvariable=name)
    surname_entry = Entry(textvariable=surname)
    passw_entry = Entry(textvariable=passw)

    name_entry.grid(row=0,column=9, padx=5, pady=5)
    surname_entry.grid(row=1,column=9, padx=5, pady=5)
    passw_entry.grid(row=2, column=9,padx=5,pady=5)


    message_button = Button(text="Ввести", command=Button_on)
    message_button.grid(row=2,column='10',padx=10, pady=5, sticky="e")
    if  but==1:
        if name.get() + " " + surname.get() == admin and passw.get()==passadmin and but==1:
            display_correct_admin()
        elif name.get() + " " + surname.get() == name1 or name.get() + " " + surname.get()==name2 and but==1:
            messagebox.showinfo("SmartHouse SOFTWARE", "Добро пожаловать, любимая Манечкаа!!")
            clean_display2()
        else:
            display_incorrect()
def Log_menu():
    clean_display1()
    display2()
def clean_display2():
    destroy_object = [name_label, name_entry, surname_label,surname_entry, message_button, passw_entry,passw_label]
    for object_name in destroy_object:
        object_name.destroy()


def display_correct_admin():
    messagebox.showinfo("SmartHouse SOFTWARE", "Welcome, dear admin!!")
    clean_display2()
def display_correct():
    messagebox.showinfo("SmartHouse SOFTWARE", "Welcome, dear " + name.get()+"!")
    clean_display2()
def display_incorrect():
    messagebox.showinfo("SmartHouse SOFTWARE", "Неправильно введены данные для входа в систему, попробуйте еще :( ")
