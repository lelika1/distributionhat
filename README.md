# DistributionHat
DistributionHat - система приема документов при поступлении в университет

# DistributionHat REST API

## Get Faculties list
Возвращает информацию о факультетах, на которые в текущий момент можно пройти с заданным средним баллом *mark*. Список формируется следующим образом: для каждого факультета считается количество уже поданных заявок со средним баллом большим или равным *mark*. Если количество таких заявок меньше *slotsCount* (набор в этом году), то данный факультет добавляется в список.
Если параметр *mark* не задан или задан некорректно, то возвращается информация о всех факультетах.

* **REQUEST**
    * **URL:** `/faculties`
    * **Method:** `GET`
    * **Parameters:** `mark=315`
* **RESPONSE**
    * **Content-Type:** `application/json`
    * **Code:** `200 HTTP_OK`
    * **Body:**
    ```
    {
        "faculties": [
            {
                "name": "Faculty name",
                "id": "Faculty GUID",
                "slotsCount": 10,
                "applicationsCount": 25
            },
            ...
        ]
    }
    ```

## Get Faculty Info

Возвращает информацию о запрашиваемом факультете: название факультета, аббревиатура, краткое описание, набор на факультет в этом году, количество поданных на данный момент заявок и гистограмма текущего распределения заявок.
Для построения гистограммы диапазон допустимых значений среднего балла делится на интервалы, для каждого из которых считается количество попавших в данный интервал заявок. Гистограмма передается в виде массива объектов, у которых *markMin* и *markMax* - границы интервала, *applications* - количество заявок, у которых средний балл попадает в данные границы.

* **REQUEST**
    * **URL:** `/faculties/(faculty_id)`
    * **Method:** `GET`
* **RESPONSE**
    * **Content-Type:** `application/json`
    * **Code:**
        * Success - `200 HTTP_OK`
        * Fail - `404 HTTP_NOT_FOUND`
    * **Body:** (will be empty, if faculty was not found)
    ```
    {
        "name": "Faculty name",
        "id": "Faculty GUID",
        "description": "Some information about faculty",
        "slotsCount": 10,
        "applicationsCount": 25,
        "distributionHist": [
            {
                "markMin": 300,
                "markMax": 310,
                "applications": 2
            },
            ...
        ]
    }
    ```

## Create New Application
Добавляет заявку на выбранный факультет (заявка представляет собой номер паспорта, и средний балл). Если в системе (на любом из факультетов) числится заявка с таким же номером паспорта, на запрос возвращается ошибка.

* **REQUEST**
    * **URL:** `/faculties/(faculty_id)`
    * **Method:** `POST`
    * **Body:**
    ```
    {
        "passport": "AB00000",
        "mark": 317
    }
    ```
* **RESPONSE**
    * **Code:**
        * Success - `200 HTTP_OK`
        * Fail - `400 HTTP_BAD_REQUEST`


## Delete Application

* **REQUEST**
    * **URL:**  `/faculties/(faculty_id)/(passport)`
    * **Method:** `DELETE`
* **RESPONSE**
    * **Code:**
        * Success - `200 HTTP_OK`
        * Fail    - `404 HTTP_NOT_FOUND`
