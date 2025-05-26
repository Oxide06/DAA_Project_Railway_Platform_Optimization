import csv
import random
from datetime import datetime, timedelta

def generate_random_time():
    hour = random.randint(0, 23)
    minute = random.randint(0, 59)
    second = random.randint(0, 59)
    return datetime(2000, 1, 1, hour, minute, second)

def format_time(dt):
    return dt.strftime("%H:%M:%S")

def generate_train_schedule(filename, num_trains=150):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        
        for i in range(1, num_trains + 1):
            train_id = f"T{i}"
            arrival_time = generate_random_time()
            min_offset_seconds = 3  # minimum 3 seconds between arrival and departure
            max_offset_minutes = 90
            offset_seconds = random.randint(min_offset_seconds, max_offset_minutes * 60)
            departure_offset = timedelta(seconds=offset_seconds)
            departure_time = arrival_time + departure_offset

            writer.writerow([
                train_id,
                format_time(arrival_time),
                format_time(departure_time)
            ])

    print(f"Generated {filename} with {num_trains} trains.")

# Generate the CSV
generate_train_schedule("train_schedule.csv")
