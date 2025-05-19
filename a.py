import csv
import random
from datetime import datetime, timedelta

def generate_random_time():
    hour = random.randint(0, 23)
    minute = random.randint(0, 59)
    return datetime(2000, 1, 1, hour, minute)

def format_time(dt):
    return dt.strftime("%H:%M")

def generate_train_schedule(filename, num_trains=150):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        
        for i in range(1, num_trains + 1):
            train_id = f"T{i}"
            arrival_time = generate_random_time()
            departure_offset = timedelta(minutes=random.randint(10, 90))
            departure_time = arrival_time + departure_offset

            writer.writerow([
                train_id,
                format_time(arrival_time),
                format_time(departure_time)
            ])

    print(f"Generated {filename} with {num_trains} trains.")

# Generate the CSV
generate_train_schedule("train_schedule.csv")
