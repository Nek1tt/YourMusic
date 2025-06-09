CREATE DATABASE  IF NOT EXISTS `yourmusic` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `yourmusic`;
-- MySQL dump 10.13  Distrib 8.0.42, for Win64 (x86_64)
--
-- Host: localhost    Database: yourmusic
-- ------------------------------------------------------
-- Server version	8.0.42

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `usertag` varchar(45) NOT NULL,
  `username` varchar(50) NOT NULL,
  `email` varchar(100) NOT NULL,
  `password` varchar(255) NOT NULL,
  `profile_blob` longblob,
  `myloadedtracks` int DEFAULT '1',
  `mytracks` int DEFAULT '1',
  `myalbums` int DEFAULT '1',
  `myloadedalbums` int DEFAULT '1',
  PRIMARY KEY (`usertag`),
  UNIQUE KEY `email` (`email`),
  UNIQUE KEY `usertag_UNIQUE` (`usertag`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES ('1','1','1','1',NULL,1,1,1,1),('12','1','11','1',NULL,1,1,1,1),('alice','Alice','alice@example.com','pass123',_binary '/images/alice.jpg',1,1,1,1),('alyosha','alyoshaabdyldaeva','alyoshaspictures@yandex.ru','rjnktnrf2003',NULL,1,1,1,1),('Author 2','userrag ','of','1',NULL,1,1,1,1),('bob','Bob','bob@example.com','pass123',_binary '/images/bob.jpg',1,1,1,1),('ellen','Ellen','ellen@example.com','pass123',_binary '/images/ellen.jpg',1,1,1,1),('empty_artist','Empty','empty@example.com','pass123',_binary '/images/empty.jpg',1,1,1,1),('frank','Frank','frank@example.com','pass123',_binary '/images/frank.jpg',1,1,1,1),('gina','Gina','gina@example.com','pass123',_binary '/images/gina.jpg',1,1,1,1),('goshan','Goshan','goshan@example.com','pass123',_binary '/images/goshan.jpg',1,1,1,1),('harry','Harry','harry@example.com','pass123',_binary '/images/harry.jpg',1,1,1,1),('ivan','Ivan','ivan@example.com','pass123',_binary '/images/ivan.jpg',1,1,1,1),('jack','Jack','jack@example.com','pass123',_binary '/images/jack.jpg',1,1,1,1),('justdice','JustDice','just@example.com','pass123',NULL,1,1,1,1),('kate','Kate','kate@example.com','pass123',_binary '/images/kate.jpg',1,1,1,1),('leo','Leo','leo@example.com','pass123',_binary '/images/leo.jpg',1,1,1,1),('mike','Mike','mike@example.com','pass123',_binary '/images/mike.jpg',1,1,1,1),('nancy','Nancy','nancy@example.com','pass123',_binary '/images/nancy.jpg',1,1,1,1),('nick','Nick','nick@example.com','pass123',_binary '/images/nick.jpg',1,1,1,1),('nikita','Nikita','nikita@example.com','pass123',_binary '/images/nikita.jpg',1,1,1,1),('Nikitaa','Nikita','Nikita','Nikita',NULL,1,1,1,1),('nurshat','Nurshat','nurshat@example.com','pass123',_binary '/images/nurshat.jpg',1,1,1,1),('oscar','Oscar','oscar@example.com','pass123',_binary '/images/oscar.jpg',1,1,1,1),('stas','Stas','stas@example.com','pass123',_binary '/images/stas.jpg',1,1,1,1),('воркакашек','воркакашек','воркакашек','воркакашек',NULL,1,1,1,1),('нуршаттег','нуршат ахуеннный','нуршат.имейл','супербезопасныйпраольневзломатьюком',NULL,1,1,1,1);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-06-09  0:48:52
