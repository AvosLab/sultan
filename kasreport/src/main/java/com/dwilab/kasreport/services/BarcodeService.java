/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.dwilab.kasreport.services;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import net.sf.jasperreports.engine.JRException;
import net.sf.jasperreports.engine.JasperCompileManager;
import net.sf.jasperreports.engine.JasperFillManager;
import net.sf.jasperreports.engine.JasperPrint;
import net.sf.jasperreports.engine.JasperReport;
import net.sf.jasperreports.engine.design.JasperDesign;
import net.sf.jasperreports.engine.xml.JRXmlLoader;
import net.sf.jasperreports.view.JasperViewer;

/**
 *
 * @author dwi
 */
public class BarcodeService {
    public void generateBarcodeFromItem() throws IOException {
        File pdfFile = File.createTempFile("item-barcode", ".pdf");
        try(FileInputStream pos = new FileInputStream(pdfFile)){
            ClassLoader classLoader = BarcodeService.class.getClassLoader();  
            URL resource = classLoader.getResource("jasper/barcode.jrxml");  
            String path = resource.getPath();
            
            final JasperReport report = JasperCompileManager.compileReport(path);
            JasperPrint print = JasperFillManager.fillReport(report, null);
            JasperViewer.viewReport(print);
        } catch (final Exception e){
            System.out.println(e.getMessage());
        }
    }
}
