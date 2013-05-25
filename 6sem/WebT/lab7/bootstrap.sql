DROP DATABASE IF EXISTS `sem6_java_bank_belarusbank`;

CREATE DATABASE IF NOT EXISTS `sem6_java_bank_belarusbank`;
USE `sem6_java_bank_belarusbank`;

CREATE TABLE accounts (`account_no` int PRIMARY KEY AUTO_INCREMENT,
                       `amount` bigint NOT NULL DEFAULT 0)
       ENGINE=InnoDB;

INSERT INTO `accounts` (`account_no`, `amount`)
       VALUES (1, 10),
              (2, 100),
              (3, 10000);

DROP DATABASE IF EXISTS `sem6_java_bank_priorbank`;

CREATE DATABASE IF NOT EXISTS `sem6_java_bank_priorbank`;
USE `sem6_java_bank_priorbank`;

CREATE TABLE accounts (`account_no` int PRIMARY KEY AUTO_INCREMENT,
                       `amount` bigint NOT NULL DEFAULT 0)
       ENGINE=InnoDB;

INSERT INTO `accounts` (`account_no`, `amount`)
       VALUES (1, 500),
              (2, 50),
              (3, 5000);
