import requests
import json
from selenium import webdriver
from selenium.webdriver.firefox.options import Options
from selenium.webdriver.firefox.service import Service
from webdriver_manager.firefox import GeckoDriverManager
from bs4 import BeautifulSoup

def search_shoe(url, driver):
    #setup the stats for an individual shoe
    stats = {}
    
    print(url)

    driver.get(url)

    html_content = driver.page_source
    
    soup = BeautifulSoup(html_content, 'html.parser')

    # I wonder if I can just compress these lines to get att all the trs 
    shoe_view = soup.find('div', class_='rr_section_v2')  
    table = shoe_view.find_all('table')
    body = table[0].find('tbody')
    rows = body.find_all('tr')

    shoe_view_name = soup.find('div', class_='shoe-review')  
    table_name = shoe_view_name.find('table')


    # get the name
    thead = table_name.find('thead')
    tr = thead.find('tr')
    ths = tr.find_all('th')
    name = ths[1].text.strip() if len(ths) > 1 else None 
    stats['name'] = name

    for row in rows:
        stat_name = row.find('th').text.strip()
        if row.find('td') != None:
            stats[stat_name]= row.find('td').text.strip()
    return (stats)


if __name__ == "__main__":
    shoe_stats = []
    urls = './runningShoes.txt'
    
    options = Options()
    options.add_argument("-headless")
    
    service = Service(executable_path=GeckoDriverManager().install())
    driver = webdriver.Firefox(service=service, options=options)
 
    with open(urls, 'r') as file:
        for line in file:
            shoe = search_shoe(line, driver)
            if shoe != None:
                shoe_stats.append(shoe)
    
    driver.quit()

    #write this directly without using print in case that is causing issues
    json_str = json.dumps(shoe_stats, indent=4)
    with open('./man_shoes.json', 'w') as file:
        file.write(json_str)


