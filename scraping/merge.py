import json

with open('man_shoes.json') as f1, open('shoes.json') as f2:
    lab_data = json.load(f2)
    company_data = json.load(f1)

lab_data_dict = {item['name']: item for item in lab_data}

#merge based on name
merged_data = []
for item in company_data:
    shared_id = item['name']
    if shared_id in lab_data_dict:
        merged_item = {**item, **lab_data_dict[shared_id]}
        merged_data.append(merged_item)

with open('all_shoes.json', 'w') as f3:
    json.dump(merged_data, f3, indent=2)