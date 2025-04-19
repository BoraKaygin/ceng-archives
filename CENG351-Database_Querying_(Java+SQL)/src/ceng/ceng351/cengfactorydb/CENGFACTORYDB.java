package ceng.ceng351.cengfactorydb;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

@SuppressWarnings("CallToPrintStackTrace")
public class CENGFACTORYDB implements ICENGFACTORYDB{

    private static Connection connection = null;
    /**
     * Place your initialization code inside if required.
     *
     * <p>
     * This function will be called before all other operations. If your implementation
     * need initialization , necessary operations should be done inside this function. For
     * example, you can set your connection to the database server inside this function.
     */
    public void initialize() {
        String url = "jdbc:mysql://144.122.71.128:8080/db2580702";

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection =  DriverManager.getConnection(url, "e2580702", "Lo326$CPowxj");
        }
        catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    /**
     * Should create the necessary tables when called.
     *
     * @return the number of tables that are created successfully.
     */
    public int createTables() {
        int result = 0;
        String createFactoryStr =   "CREATE TABLE Factory (" +
                                    "factoryId INT," +
                                    "factoryName VARCHAR(255)," +
                                    "factoryType VARCHAR(255)," +
                                    "country VARCHAR(255)," +
                                    "PRIMARY KEY (factoryId)" +
                                    ");";
        String createEmployeeStr =  "CREATE TABLE Employee (" +
                                    "employeeId INT," +
                                    "employeeName VARCHAR(255)," +
                                    "department VARCHAR(255)," +
                                    "salary INT," +
                                    "PRIMARY KEY (employeeId)" +
                                    ");";
        String createWorksInStr =   "CREATE TABLE Works_In (" +
                                    "factoryId INT," +
                                    "employeeId INT," +
                                    "startDate DATE," +
                                    "PRIMARY KEY (factoryId,employeeId)," +
                                    "FOREIGN KEY (employeeId) REFERENCES Employee(employeeId) ON DELETE CASCADE," +
                                    "FOREIGN KEY (factoryId) REFERENCES Factory(factoryId)" +
                                    ");";
        String createProductStr =   "CREATE TABLE Product (" +
                                    "productId INT," +
                                    "productName VARCHAR(255)," +
                                    "productType VARCHAR(255)," +
                                    "PRIMARY KEY (productId)" +
                                    ");";
        String createProduceStr =   "CREATE TABLE Produce (" +
                                    "factoryId INT," +
                                    "productId INT," +
                                    "amount INT," +
                                    "productionCost INT," +
                                    "PRIMARY KEY (factoryId,productId)," +
                                    "FOREIGN KEY (factoryId) REFERENCES Factory(factoryId)," +
                                    "FOREIGN KEY (productId) REFERENCES Product(productId)" +
                                    ");";
        String createShipmentStr =  "CREATE TABLE Shipment (" +
                                    "factoryId INT," +
                                    "productId INT," +
                                    "amount INT," +
                                    "pricePerUnit INT," +
                                    "PRIMARY KEY (factoryId,productId)," +
                                    "FOREIGN KEY (factoryId) REFERENCES Factory(factoryId)," +
                                    "FOREIGN KEY (productId) REFERENCES Product(productId)" +
                                    ");";
        try {
            PreparedStatement createFactory = connection.prepareStatement(createFactoryStr);
            createFactory.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            PreparedStatement createEmployee = connection.prepareStatement(createEmployeeStr);
            createEmployee.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            PreparedStatement createWorksIn = connection.prepareStatement(createWorksInStr);
            createWorksIn.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            PreparedStatement createProduct = connection.prepareStatement(createProductStr);
            createProduct.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            PreparedStatement createProduce = connection.prepareStatement(createProduceStr);
            createProduce.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            PreparedStatement createShipment = connection.prepareStatement(createShipmentStr);
            createShipment.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return result;
    }

    /**
     * Should drop the tables if exists when called.
     *
     * @return the number of tables are dropped successfully.
     */
    public int dropTables() {
        int result = 0;
        String dropFactoryStr = "DROP TABLE Factory;";
        String dropEmployeeStr = "DROP TABLE Employee;";
        String dropWorksInStr = "DROP TABLE Works_In;";
        String dropProductStr = "DROP TABLE Product;";
        String dropProduceStr = "DROP TABLE Produce;";
        String dropShipmentStr = "DROP TABLE Shipment;";
        try {
            PreparedStatement dropShipment = connection.prepareStatement(dropShipmentStr);
            dropShipment.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        try {
            PreparedStatement dropProduce = connection.prepareStatement(dropProduceStr);
            dropProduce.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        try {
            PreparedStatement dropProduct = connection.prepareStatement(dropProductStr);
            dropProduct.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        try {
            PreparedStatement dropWorksIn = connection.prepareStatement(dropWorksInStr);
            dropWorksIn.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        try {
            PreparedStatement dropEmployee = connection.prepareStatement(dropEmployeeStr);
            dropEmployee.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        try {
            PreparedStatement dropFactory = connection.prepareStatement(dropFactoryStr);
            dropFactory.executeUpdate();
            result++;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return result;
    }

    /**
     * Should insert an array of Factory into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertFactory(Factory[] factories) {
        int result = 0;
        String insertFactoryStr = "INSERT INTO Factory (factoryId, factoryName, factoryType, country) VALUES (?,?,?,?);";
        for (Factory factory : factories){
            try {
                PreparedStatement insertFactory = connection.prepareStatement(insertFactoryStr);
                insertFactory.setInt(1, factory.getFactoryId());
                insertFactory.setString(2, factory.getFactoryName());
                insertFactory.setString(3, factory.getFactoryType());
                insertFactory.setString(4, factory.getCountry());
                insertFactory.executeUpdate();
                result++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return result;
    }

    /**
     * Should insert an array of Employee into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertEmployee(Employee[] employees) {
        int result = 0;
        String insertEmployeeStr = "INSERT INTO Employee (employeeId, employeeName, department, salary) VALUES (?,?,?,?);";
        for (Employee employee : employees){
            try {
                PreparedStatement insertEmployee = connection.prepareStatement(insertEmployeeStr);
                insertEmployee.setInt(1, employee.getEmployeeId());
                insertEmployee.setString(2, employee.getEmployeeName());
                insertEmployee.setString(3, employee.getDepartment());
                insertEmployee.setInt(4, employee.getSalary());
                insertEmployee.executeUpdate();
                result++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return result;
    }

    /**
     * Should insert an array of WorksIn into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertWorksIn(WorksIn[] worksIns) {
        int result = 0;
        String insertWorksInStr = "INSERT INTO Works_In (factoryId, employeeId, startDate) VALUES (?,?,?);";
        for (WorksIn worksIn : worksIns){
            try {
                PreparedStatement insertWorksIn = connection.prepareStatement(insertWorksInStr);
                insertWorksIn.setInt(1, worksIn.getFactoryId());
                insertWorksIn.setInt(2, worksIn.getEmployeeId());
                insertWorksIn.setString(3, worksIn.getStartDate());
                insertWorksIn.executeUpdate();
                result++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return result;
    }

    /**
     * Should insert an array of Product into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertProduct(Product[] products) {
        int result = 0;
        String insertProductStr = "INSERT INTO Product (productId, productName, productType) VALUES (?,?,?);";
        for (Product product : products){
            try {
                PreparedStatement insertProduct = connection.prepareStatement(insertProductStr);
                insertProduct.setInt(1, product.getProductId());
                insertProduct.setString(2, product.getProductName());
                insertProduct.setString(3, product.getProductType());
                insertProduct.executeUpdate();
                result++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return result;
    }


    /**
     * Should insert an array of Produce into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertProduce(Produce[] produces) {
        int result = 0;
        String insertProduceStr = "INSERT INTO Produce (factoryId, productId, amount, productionCost) VALUES (?,?,?,?);";
        for (Produce produce : produces){
            try {
                PreparedStatement insertProduce = connection.prepareStatement(insertProduceStr);
                insertProduce.setInt(1, produce.getFactoryId());
                insertProduce.setInt(2, produce.getProductId());
                insertProduce.setInt(3, produce.getAmount());
                insertProduce.setInt(4, produce.getProductionCost());
                insertProduce.executeUpdate();
                result++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return result;
    }


    /**
     * Should insert an array of Shipment into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertShipment(Shipment[] shipments) {
        int result = 0;
        String insertShipmentStr = "INSERT INTO Shipment (factoryId, productId, amount, pricePerUnit) VALUES (?,?,?,?);";
        for (Shipment shipment : shipments){
            try {
                PreparedStatement insertShipment = connection.prepareStatement(insertShipmentStr);
                insertShipment.setInt(1, shipment.getFactoryId());
                insertShipment.setInt(2, shipment.getProductId());
                insertShipment.setInt(3, shipment.getAmount());
                insertShipment.setInt(4, shipment.getPricePerUnit());
                insertShipment.executeUpdate();
                result++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return result;
    }

    /**
     * Should return all factories that are located in a particular country.
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesForGivenCountry(String country) {
        ResultSet resultSet;
        Factory[] factories = null;
        String queryStr =   "SELECT DISTINCT * " +
                            "FROM Factory " +
                            "WHERE country = ? " +
                            "ORDER BY factoryId;";
        try {
            PreparedStatement getFactoriesForGivenCountry = connection.prepareStatement(queryStr);
            getFactoriesForGivenCountry.setString(1, country);
            resultSet = getFactoriesForGivenCountry.executeQuery();
            resultSet.last();
            factories = new Factory[resultSet.getRow()];
            resultSet.beforeFirst();
            int i = 0;
            while (resultSet.next()){
                factories[i] = new Factory(resultSet.getInt("factoryId"),
                                           resultSet.getString("factoryName"),
                                           resultSet.getString("factoryType"),
                                           resultSet.getString("country"));
                i++;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return factories;
    }



    /**
     * Should return all factories without any working employees.
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesWithoutAnyEmployee() {
        ResultSet resultSet;
        Factory[] factories = null;
        String queryStr =   "SELECT DISTINCT * " +
                            "FROM Factory F " +
                            "WHERE NOT EXISTS " +
                                "(SELECT * " +
                                "FROM Works_In W " +
                                "WHERE F.factoryId = W.factoryId) " +
                            "ORDER BY F.factoryId;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            resultSet = query.executeQuery();
            resultSet.last();
            factories = new Factory[resultSet.getRow()];
            resultSet.beforeFirst();
            int i = 0;
            while (resultSet.next()){
                factories[i] = new Factory(resultSet.getInt("factoryId"),
                        resultSet.getString("factoryName"),
                        resultSet.getString("factoryType"),
                        resultSet.getString("country"));
                i++;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return factories;
    }

    /**
     * Should return all factories that produce all products for a particular productType
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesProducingAllForGivenType(String productType) {
        List<Factory> factories = new ArrayList<>();
        String queryStr =
                "SELECT * " +
                "FROM Factory F " +
                "WHERE NOT EXISTS( " +
                "    (SELECT P.productId " +
                "    FROM Product P " +
                "    WHERE P.productType = ?) " +
                "    EXCEPT " +
                "    (SELECT produce.productId " +
                "    FROM Produce produce " +
                "    WHERE produce.factoryId = F.factoryId) " +
                ") ORDER BY factoryId ASC;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            query.setString(1, productType);
            ResultSet resultSet = query.executeQuery();
            while(resultSet.next()){
                int factoryId = resultSet.getInt("factoryId");
                String factoryName = resultSet.getString("factoryName");
                String factoryType = resultSet.getString("factoryType");
                String factoryCountry = resultSet.getString("country");
                Factory factory = new Factory(factoryId, factoryName, factoryType, factoryCountry);
                factories.add(factory);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return factories.toArray(new Factory[0]);
        /*
        ResultSet resultSet;
        Factory[] factories = null;
        String queryStr =   "SELECT DISTINCT * " +
                            "FROM Factory F " +
                            "WHERE NOT EXISTS " +
                                "((SELECT P.productId " +
                                "FROM Product P " +
                                "WHERE P.productType = ?) " +
                                "EXCEPT " +
                                "(SELECT Pr.productId " +
                                "FROM Produce Pr " +
                                "WHERE Pr.factoryId = F.factoryId)) " +
                            "ORDER BY F.factoryId;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            query.setString(1, productType);
            resultSet = query.executeQuery();
            resultSet.last();
            factories = new Factory[resultSet.getRow()];
            resultSet.beforeFirst();
            int i = 0;
            while (resultSet.next()){
                factories[i] = new Factory(resultSet.getInt("factoryId"),
                        resultSet.getString("factoryName"),
                        resultSet.getString("factoryType"),
                        resultSet.getString("country"));
                i++;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return factories;
        */
    }


    /**
     * Should return the products that are produced in a particular factory but
     * don’t have any shipment from that factory.
     *
     * @return Product[]
     */
    public Product[] getProductsProducedNotShipped() {
        ResultSet resultSet;
        Product[] products = null;
        String queryStr =   "SELECT DISTINCT P.productID, P.productName, P.productType " +
                            "FROM Product P " +
                            "WHERE EXISTS " +
                                "((Select Pr.factoryId " +
                                "FROM Produce Pr " +
                                "WHERE P.productId = Pr.productId) " +
                                "EXCEPT " +
                                "(Select S.factoryId " +
                                "FROM Shipment S " +
                                "WHERE P.productId = S.productId)) " +
                            "ORDER BY P.productId;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            resultSet = query.executeQuery();
            resultSet.last();
            products = new Product[resultSet.getRow()];
            resultSet.beforeFirst();
            int i = 0;
            while (resultSet.next()){
                products[i] = new Product(resultSet.getInt("productId"),
                        resultSet.getString("productName"),
                        resultSet.getString("productType"));
                i++;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return products;
    }


    /**
     * For a given factoryId and department, should return the average salary of
     *     the employees working in that factory and that specific department.
     *
     * @return double
     */
    public double getAverageSalaryForFactoryDepartment(int factoryId, String department) {
        ResultSet resultSet;
        double averageSalary = 0;
        String queryStr =   "SELECT AVG(E.salary) " +
                            "FROM Employee E, Works_In W " +
                            "WHERE E.employeeId = W.employeeId AND W.factoryId = ? AND E.department = ?;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            query.setInt(1, factoryId);
            query.setString(2, department);
            resultSet = query.executeQuery();
            resultSet.next();
            averageSalary = resultSet.getDouble(1);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return averageSalary;
    }


    /**
     * Should return the most profitable products for each factory
     *
     * @return QueryResult.MostValueableProduct[]
     */
    public QueryResult.MostValueableProduct[] getMostValueableProducts() {
        ResultSet resultSet;
        QueryResult.MostValueableProduct[] mostValueableProducts = null;
        String queryStr =   "SELECT DISTINCT T.factoryId, T.productId, T.productName, T.productType, T.profit " +
                            "FROM (SELECT Pr.factoryId, P.productId, P.productName, P.productType, (COALESCE(S.amount,0) * COALESCE(S.pricePerUnit,0) - Pr.amount * Pr.productionCost) AS profit " +
                                "FROM Product P, Produce Pr LEFT OUTER JOIN Shipment S ON (Pr.factoryId = S.factoryId AND Pr.productId = S.productId) " +
                                "WHERE P.productId = Pr.productId) AS T, " +
                                "(SELECT Pr1.factoryId, MAX(COALESCE(S1.amount,0) * COALESCE(S1.pricePerUnit,0) - Pr1.amount * Pr1.productionCost) AS profit " +
                                "FROM Produce Pr1 LEFT OUTER JOIN Shipment S1 ON (Pr1.factoryId = S1.factoryId AND Pr1.productId = S1.productId) " +
                                "GROUP BY Pr1.factoryId) AS T1 " +
                            "WHERE T.factoryId = T1.factoryId AND " +
                                "T.profit = T1.profit " +
                            "ORDER BY T.profit DESC, T.factoryId;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            resultSet = query.executeQuery();
            resultSet.last();
            mostValueableProducts = new QueryResult.MostValueableProduct[resultSet.getRow()];
            resultSet.beforeFirst();
            int i = 0;
            while (resultSet.next()){
                mostValueableProducts[i] = new QueryResult.MostValueableProduct(resultSet.getInt("factoryId"),
                        resultSet.getInt("productId"),
                        resultSet.getString("productName"),
                        resultSet.getString("productType"),
                        resultSet.getInt("profit"));
                i++;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return mostValueableProducts;
    }

    /**
     * For each product, return the factories that gather the highest profit
     * for that product
     *
     * @return QueryResult.MostValueableProduct[]
     */
    public QueryResult.MostValueableProduct[] getMostValueableProductsOnFactory() {
        ResultSet resultSet;
        QueryResult.MostValueableProduct[] mostValueableProducts = null;
        String queryStr =   "SELECT DISTINCT T.factoryId, T.productId, T.productName, T.productType, T.profit " +
                            "FROM (SELECT Pr.factoryId, P.productId, P.productName, P.productType, (COALESCE(S.amount,0) * COALESCE(S.pricePerUnit,0) - Pr.amount * Pr.productionCost) AS profit " +
                                "FROM Product P, Produce Pr LEFT OUTER JOIN Shipment S ON (Pr.factoryId = S.factoryId AND Pr.productId = S.productId) " +
                                "WHERE P.productId = Pr.productId) AS T, " +
                                "(SELECT Pr1.productId, MAX(COALESCE(S1.amount,0) * COALESCE(S1.pricePerUnit,0) - Pr1.amount * Pr1.productionCost) AS profit " +
                                "FROM Produce Pr1 LEFT OUTER JOIN Shipment S1 ON (Pr1.factoryId = S1.factoryId AND Pr1.productId = S1.productId) " +
                                "GROUP BY Pr1.productId) AS T1 " +
                            "WHERE T.productId = T1.productId AND " +
                                "T.profit = T1.profit " +
                            "ORDER BY T.profit DESC, T.productId;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            resultSet = query.executeQuery();
            resultSet.last();
            mostValueableProducts = new QueryResult.MostValueableProduct[resultSet.getRow()];
            resultSet.beforeFirst();
            int i = 0;
            while (resultSet.next()){
                mostValueableProducts[i] = new QueryResult.MostValueableProduct(resultSet.getInt("factoryId"),
                        resultSet.getInt("productId"),
                        resultSet.getString("productName"),
                        resultSet.getString("productType"),
                        resultSet.getInt("profit"));
                i++;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return mostValueableProducts;
    }


    /**
     * For each department, should return all employees that are paid under the
     *     average salary for that department. You consider the employees
     *     that do not work earning "0".
     *
     * @return QueryResult.LowSalaryEmployees[]
     */
    public QueryResult.LowSalaryEmployees[] getLowSalaryEmployeesForDepartments() {
        ResultSet resultSet;
        QueryResult.LowSalaryEmployees[] lowSalaryEmployees = null;
        String queryStr =   "SELECT DISTINCT T.employeeId, T.employeeName, T.department, T.salary " +
                            "FROM (SELECT E.employeeId, E.employeeName, E.department, (IF(W.startDate IS NULL, 0, E.salary)) AS salary " +
                                "FROM Employee E NATURAL LEFT OUTER JOIN Works_In W) AS T, " +
                                "(SELECT E1.department, AVG(IF(W1.startDate IS NULL, 0, E1.salary)) AS avgSalary " +
                                "FROM Employee E1 NATURAL LEFT OUTER JOIN Works_In W1 " +
                                "GROUP BY E1.department) AS T1 " +
                            "WHERE T.department = T1.department AND " +
                                "T.salary < T1.avgSalary " +
                            "ORDER BY T.employeeId;";
        try {
            PreparedStatement query = connection.prepareStatement(queryStr);
            resultSet = query.executeQuery();
            resultSet.last();
            lowSalaryEmployees = new QueryResult.LowSalaryEmployees[resultSet.getRow()];
            resultSet.beforeFirst();
            int i = 0;
            while (resultSet.next()){
                lowSalaryEmployees[i] = new QueryResult.LowSalaryEmployees(resultSet.getInt("employeeId"),
                        resultSet.getString("employeeName"),
                        resultSet.getString("department"),
                        resultSet.getInt("salary"));
                i++;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return lowSalaryEmployees;
    }


    /**
     * For the products of given productType, increase the productionCost of every unit by a given percentage.
     *
     * @return number of rows affected
     */
    public int increaseCost(String productType, double percentage) {
        int result = 0;
        String updateStr =  "UPDATE Produce Pr " +
                            "SET Pr.productionCost = Pr.productionCost + Pr.productionCost * ? " +
                            "WHERE Pr.productId = ANY " +
                                "(SELECT P.productId " +
                                "FROM Product P " +
                                "WHERE P.productType = ?);";
        try {
            PreparedStatement update = connection.prepareStatement(updateStr);
            update.setDouble(1, percentage);
            update.setString(2, productType);
            result = update.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return result;
    }


    /**
     * Deleting all employees that have not worked since the given date.
     *
     * @return number of rows affected
     */
    public int deleteNotWorkingEmployees(String givenDate) {
        int result = 0;
        String updateStr =  "DELETE FROM Employee " +
                            "WHERE NOT EXISTS " +
                                "(SELECT * " +
                                "FROM Works_In W " +
                                "WHERE Employee.employeeId = W.employeeId AND W.startDate >= ?);";
        try {
            PreparedStatement update = connection.prepareStatement(updateStr);
            update.setString(1, givenDate);
            result = update.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return result;
    }


    /**
     * *****************************************************
     * *****************************************************
     * *****************************************************
     * *****************************************************
     *  THE METHODS AFTER THIS LINE WILL NOT BE GRADED.
     *  YOU DON'T HAVE TO SOLVE THEM, LEAVE THEM AS IS IF YOU WANT.
     *  IF YOU HAVE ANY QUESTIONS, REACH ME VIA EMAIL.
     * *****************************************************
     * *****************************************************
     * *****************************************************
     * *****************************************************
     */

    /**
     * For each department, find the rank of the employees in terms of
     * salary. Peers are considered tied and receive the same rank. After
     * that, there should be a gap.
     *
     * @return QueryResult.EmployeeRank[]
     */
    public QueryResult.EmployeeRank[] calculateRank() {
        return new QueryResult.EmployeeRank[0];
    }

    /**
     * For each department, find the rank of the employees in terms of
     * salary. Everything is the same but after ties, there should be no
     * gap.
     *
     * @return QueryResult.EmployeeRank[]
     */
    public QueryResult.EmployeeRank[] calculateRank2() {
        return new QueryResult.EmployeeRank[0];
    }

    /**
     * For each factory, calculate the most profitable 4th product.
     *
     * @return QueryResult.FactoryProfit
     */
    public QueryResult.FactoryProfit calculateFourth() {
        return new QueryResult.FactoryProfit(0,0,0);
    }

    /**
     * Determine the salary variance between an employee and another
     * one who began working immediately after the first employee (by
     * startDate), for all employees.
     *
     * @return QueryResult.SalaryVariant[]
     */
    public QueryResult.SalaryVariant[] calculateVariance() {
        return new QueryResult.SalaryVariant[0];
    }

    /**
     * Create a method that is called once and whenever a Product starts
     * losing money, deletes it from Produce table
     *
     * @return void
     */
    public void deleteLosing() {

    }
}
