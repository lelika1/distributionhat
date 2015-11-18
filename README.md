# DistributionHat
DistributionHat - система приема документов при поступлении в университет

# DistributionHat REST API

## Get Faculties list
Возвращает информацию о факультетах, на которые можно пройти с указанной оценкой. Если параметр mark не задан, или задан некорректно возвращает все факультеты.

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
                "apllications": 2
            },
            ...
        ]
    }
    ```

## Create New Application

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
        * Success - `201 HTTP_CREATED`
        * Fail - `400 HTTP_BAD_REQUEST`


## Delete Application

* **REQUEST**
    * **URL:**  `/faculties/(faculty_id)/(passport)`
    * **Method:** `DELETE`
* **RESPONSE**
    * **Code:**
        * Success - `200 HTTP_OK`
        * Fail    - `404 HTTP_NOT_FOUND`
