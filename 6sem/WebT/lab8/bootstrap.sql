DROP DATABASE IF EXISTS `sem6_java_users`;

CREATE DATABASE IF NOT EXISTS `sem6_java_users`;
USE `sem6_java_users`;

CREATE TABLE users (`user_id` int PRIMARY KEY AUTO_INCREMENT,
                    `nickname` varchar(50) NOT NULL,
                    `encrypted_password` varchar(100) NOT NULL)
       ENGINE=InnoDB;

INSERT INTO `users` (`user_id`, `nickname`, `encrypted_password`)
       VALUES (1, 'test', '723d3237524c0e544c264bc8ef8fdc7e'),
              (2, 'someuser', '405623555dadb63b6f998d36b0321bbc');
