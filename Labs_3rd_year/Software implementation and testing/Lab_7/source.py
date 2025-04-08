import pytest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.action_chains import ActionChains

@pytest.fixture(scope="module")
def browser():
    # Инициализация драйвера
    driver = webdriver.Chrome()
    # Возврат драйвера тестам
    yield driver
    # Закрытие браузера
    driver.quit()


def test_error_on_negative_sum(browser):
    try:
        # Инициализация ActionChains
        actions = ActionChains(browser)

        # Открытие страницы
        browser.get("https://calcus.ru/")

        # Ожидание и прокрутка до элемента категории
        category_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".category-wrapper:nth-child(6) li:nth-child(1) > .catalog-link"))
        )
        browser.execute_script("arguments[0].scrollIntoView();", category_element)

        # Клик на элемент с помощью ActionChains
        actions.move_to_element(category_element).click().perform()

        # Ожидание появления поля для ввода суммы
        sum_input = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.NAME, "sum"))
        )
        sum_input.clear()  # Очистка поля
        sum_input.send_keys("-")

        # Ожидание появления поля для ввода ставки НДС
        rate_input = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.NAME, "rate"))
        )
        rate_input.clear()  # Очистка поля
        rate_input.send_keys("10")

        # Ожидание появления элемента выбора режима работы калькулятора
        mode_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, "div:nth-child(1) > label"))
        )
        actions.move_to_element(mode_element).click().perform()

        # Ожидание появления кнопки расчета
        submit_button = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".calc-submit"))
        )
        actions.move_to_element(submit_button).click().perform()

        # Ожидание появления элемента с ошибкой
        error_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.ID, "sum-error"))
        )
        assert error_element.text == "Обязательное поле", f"Ожидаемый текст: 'Обязательное поле', Фактический текст: '{error_element.text}'"

        # Если тест прошел успешно
        print(f"\t Тест:Проверка ошибки расчета начисления НДС при вводе спецсимвола «-» в поле ввода «Сумма» ""✅ Успех")
    except Exception as e:
        # Если тест упал
        print(f"\t Tест:Проверка ошибки расчета начисления НДС при вводе спецсимвола «-» в поле ввода «Сумма»❌ Не успех")
        raise


def test_error_on_negative_rate(browser):
    try:
        # Инициализация ActionChains
        actions = ActionChains(browser)

        # Открытие страницы
        browser.get("https://calcus.ru/")

        # Ожидание и прокрутка до элемента категории
        category_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".category-wrapper:nth-child(6) li:nth-child(1) > .catalog-link"))
        )
        browser.execute_script("arguments[0].scrollIntoView();", category_element)

        # Клик на элемент с помощью ActionChains
        actions.move_to_element(category_element).click().perform()

        # Ожидание появления поля для ввода суммы
        sum_input = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.NAME, "sum"))
        )
        sum_input.clear()  # Очистка поля
        sum_input.send_keys("10 000")

        # Ожидание появления поля для ввода ставки НДС
        rate_input = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.NAME, "rate"))
        )
        rate_input.clear()  # Очистка поля
        rate_input.send_keys("-")

        # Ожидание появления элемента выбора режима работы калькулятора
        mode_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, "div:nth-child(1) > label"))
        )
        actions.move_to_element(mode_element).click().perform()

        # Ожидание появления кнопки расчета
        submit_button = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".calc-submit"))
        )
        actions.move_to_element(submit_button).click().perform()

        # Ожидание появления элемента с ошибкой
        error_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.ID, "rate-error"))
        )
        assert error_element.text == "Обязательное поле", f"Ожидаемый текст: 'Обязательное поле', Фактический текст: '{error_element.text}'"

        # Если тест прошел успешно
        print(f"\t Тест:Проверка ошибки расчета начисления НДС при вводе спецсимвола «-» в поле ввода «Ставка НДС» ""✅ Успех")
    except Exception as e:
        # Если тест упал
        print(f"\t Тест:Проверка ошибки расчета начисления НДС при вводе спецсимвола «-» в поле ввода «Ставка НДС»❌ Не успех")
        raise


def test_redirect_to_main_page_on_logo_click(browser):
    try:
        # Инициализация ActionChains
        actions = ActionChains(browser)

        # Открытие страницы
        browser.get("https://calcus.ru/")

        # Ожидание и прокрутка до элемента категории
        category_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".category-wrapper:nth-child(6) li:nth-child(1) > .catalog-link"))
        )
        browser.execute_script("arguments[0].scrollIntoView();", category_element)

        # Клик на элемент с помощью ActionChains
        actions.move_to_element(category_element).click().perform()

        # Ожидание появления ссылки с логотипом
        logo_link = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, "a.d-lg-none"))
        )

        # Прокрутка до элемента (если он не виден)
        browser.execute_script("arguments[0].scrollIntoView({block: 'center'});", logo_link)

        # Клик по ссылке с помощью JavaScript
        browser.execute_script("arguments[0].click();", logo_link)

        header_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, "h1"))
        )

        assert header_element.text == "Онлайн калькуляторы, конвертеры и полезные инструменты", f"Ожидаемый текст: 'Онлайн калькуляторы, конвертеры и полезные инструменты', Фактический текст: '{header_element.text}'"         
        print(f"\t Тест:Проверка перехода на главную страницу при нажатие на логотип ✅ Успех")
    except Exception as e:
        # Если тест упал
        print(f"\t Тест:Проверка перехода на главную страницу при нажатие на логотип❌ Не успех")
        raise


def test_correct_calculation_with_valid_value(browser):
    try:
        # Инициализация ActionChains
        actions = ActionChains(browser)

        # Открытие страницы
        browser.get("https://calcus.ru/")

        # Ожидание и прокрутка до элемента категории
        category_locator = (By.CSS_SELECTOR, ".category-wrapper:nth-child(6) li:nth-child(1) > .catalog-link")
        category_element = WebDriverWait(browser, 15).until(
            EC.element_to_be_clickable(category_locator),
        )
        browser.execute_script("arguments[0].scrollIntoView({block: 'center'});", category_element)
        
        # Клик на элемент с помощью ActionChains
        actions.move_to_element(category_element).click().perform()

        # Ввод суммы
        sum_input = WebDriverWait(browser, 15).until(
            EC.element_to_be_clickable((By.NAME, "sum")),
        )
        sum_input.clear()
        sum_input.send_keys("10000")  

        # Ввод ставки НДС
        rate_input = WebDriverWait(browser, 15).until(
            EC.element_to_be_clickable((By.NAME, "rate")),
        )
        rate_input.clear()
        rate_input.send_keys("12")

        # Выбор режима работы калькулятора
        mode_element = WebDriverWait(browser, 15).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, "div:nth-child(1) > label")),
        )
        actions.move_to_element(mode_element).click().perform()

        # Клик на кнопку расчета
        submit_button = WebDriverWait(browser, 15).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, ".calc-submit")),
        )
        actions.move_to_element(submit_button).click().perform()

        # Явное ожидание завершения расчета
        WebDriverWait(browser, 15).until(
            EC.text_to_be_present_in_element((By.CSS_SELECTOR, ".calc-result-value"), "₽"),
        )

        # 1. Проверка суммы НДС
        number_element = WebDriverWait(browser, 15).until(
            EC.visibility_of_element_located((By.CSS_SELECTOR, ".calc-result-value.result-placeholder-before")),
        )
        nds_sum = number_element.text.replace("\u00A0", " ").strip()
        assert nds_sum == "10 000  ₽", f"Ожидалось: '10 000  ₽', Фактически: '{nds_sum}'"

        # 2. Проверка суммы НДС в деталях
        result_vat = WebDriverWait(browser, 15).until(
            EC.visibility_of_element_located((By.CSS_SELECTOR, ".result-placeholder-vat")),
        )
        vat_text = result_vat.text.replace('\u00A0', ' ').replace('  ', ' ').strip()
        assert vat_text == "1 200 ₽", f"Ожидалось: '1 200 ₽', Фактически: '{vat_text}'"

        # 3. Проверка итоговой суммы
        result_after = WebDriverWait(browser, 15).until(
            EC.visibility_of_element_located((By.CSS_SELECTOR, ".result-placeholder-after")),
        )
        total_text = result_after.text.replace('\u00A0', ' ').replace('  ', ' ').strip()
        assert total_text == "11 200 ₽", f"Ожидалось: '11 200 ₽', Фактически: '{total_text}'"

        # 4. Проверка текстового представления
        result_string = WebDriverWait(browser, 15).until(
            EC.visibility_of_element_located((By.CSS_SELECTOR, ".result-placeholder-string")),
        )
        expected_text_part = "11 200,00 рублей (одиннадцать тысяч двести рублей 00 копеек)"
        assert expected_text_part in result_string.text, f"Ожидалось содержание: '{expected_text_part}'"

        print("Тест: Проверка корректного расчета начисления НДС при вводе валидных данных ✅ УСПЕХ")
        
    except Exception as e:
        print("Тест: Проверка корректного расчета начисления НДС при вводе валидных данных ❌ ОШИБКА")
        raise


def test_check_currency_selector_on_website(browser):
    try:
        # Инициализация ActionChains
        actions = ActionChains(browser)

        # Открытие страницы
        browser.get("https://calcus.ru/")

        # Ожидание и прокрутка до элемента категории
        category_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".category-wrapper:nth-child(6) li:nth-child(1) > .catalog-link"))
        )
        browser.execute_script("arguments[0].scrollIntoView();", category_element)

        # Клик на элемент с помощью ActionChains
        actions.move_to_element(category_element).click().perform()
    
        selector_element = WebDriverWait(browser, 10).until(EC.presence_of_element_located((By.LINK_TEXT, "₽"))
        )

        print("Тест:Проверка наличия селектора валют на сайте калькулятор НДС ✅ УСПЕХ")
    except Exception as e:
        # Если тест упал
        print(f"\t Тест:Проверка наличия селектора валют на сайте калькулятор НДС❌ Не успех")
        raise


def test_test_error_report_link_present_under_calculator(browser):
    try:
        # Инициализация ActionChains
        actions = ActionChains(browser)

        # Открытие страницы
        browser.get("https://calcus.ru/")

        # Ожидание и прокрутка до элемента категории
        category_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".category-wrapper:nth-child(6) li:nth-child(1) > .catalog-link"))
        )
        browser.execute_script("arguments[0].scrollIntoView();", category_element)

        # Клик на элемент с помощью ActionChains
        actions.move_to_element(category_element).click().perform()
        element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".calc-panel-item:nth-child(3) span"))
        )
        
        # Дополнительная проверка видимости (если нужно)
        assert element.is_displayed()
        print("Тест: Проверка наличия ссылки 'Сообщить об ошибке' под блоком калькулятора ✅ УСПЕХ")
    except Exception as e:
    # Если тест не прошел
        print("\t Тест: ❌ Не успех")


def test_redirect_to_widget_page_on_link_widget(browser):
    try:
        # Инициализация ActionChains
        actions = ActionChains(browser)

        # Открытие страницы
        browser.get("https://calcus.ru/")

        # Ожидание и прокрутка до элемента категории
        category_element = WebDriverWait(browser, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, ".category-wrapper:nth-child(6) li:nth-child(1) > .catalog-link"))
        )
        browser.execute_script("arguments[0].scrollIntoView();", category_element)

        # Клик на элемент с помощью ActionChains
        actions.move_to_element(category_element).click().perform()

        element = WebDriverWait(browser, 10).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, ".calc-panel-item:nth-child(5) span"))
        ).click()
        
        # Переключение на новое окно (handle=S(win8890))
        # Получаем текущие открытые окна
        original_window = browser.current_window_handle
        WebDriverWait(browser, 10).until(EC.number_of_windows_to_be(2))
        
        # Находим новое окно (перебираем все handles)
        for window_handle in browser.window_handles:
            if window_handle != original_window:
                browser.switch_to.window(window_handle)
                break
        
        # Проверка заголовка "Калькулятор НДС - виджет для сайта"
        h1_text = WebDriverWait(browser, 10).until(
            EC.visibility_of_element_located((By.CSS_SELECTOR, "h1"))
        ).text
        
        assert h1_text == "Калькулятор НДС – виджет для сайта", \
            f"Ожидался заголовок 'Калькулятор НДС – виджет для сайта', но получили '{h1_text}'"

        print("Тест: Проверка перехода на страницу https://calcus.ru/widget/VAT при нажатии на ссылку 'Виджет' ✅ УСПЕХ")
    except Exception as e:
        # Если тест не прошел
        print(e)
        print("\t Тест: Проверка перехода на страницу https://calcus.ru/widget/VAT при нажатии на ссылку 'Виджет'❌ Не успех")

